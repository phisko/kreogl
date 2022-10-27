#include "AssImpAnimationModel.hpp"

// stl
#include <span>

// glm
#include <glm/gtc/type_ptr.hpp>

// kreogl
#include "kreogl/animation/AnimatedObject.hpp"
#include "kreogl/animation/AnimatedModel.hpp"
#include "kreogl/impl/kreogl_profiling.hpp"
#include "AssImpSkeletonModel.hpp"
#include "AssImpToGLM.hpp"

namespace kreogl {
	void AssImpAnimationModel::tick(float deltaTime, AnimatedObject & object) const noexcept {
		KREOGL_PROFILING_SCOPE;

		assert(assimpAnimation != nullptr);

		const auto & animatedModel = static_cast<const AnimatedModel &>(*object.model);
		assert(animatedModel.skeleton);

		const auto & skeletonModel = static_cast<const AssImpSkeletonModel &>(*animatedModel.skeleton);
		updateBoneMats(*skeletonModel.rootNode, object.animation->currentTime * ticksPerSecond, object, glm::mat4{ 1.f }, true);
		incrementDeltaTime(deltaTime, object);
	}

	void AssImpAnimationModel::incrementDeltaTime(float deltaTime, AnimatedObject & object) const noexcept {
		KREOGL_PROFILING_SCOPE;

		object.animation->currentTime += deltaTime * object.animation->speed;
		const auto wrappedTime = fmodf(object.animation->currentTime, totalTime);
		if (wrappedTime < object.animation->currentTime) { // We've looped
			if (object.animation->loop)
				object.animation->currentTime = wrappedTime;
			else
				object.animation = std::nullopt;
		}
	}

	namespace {
		static const aiNodeAnim * getNodeAnim(const aiAnimation * anim, const char * name) noexcept {
			KREOGL_PROFILING_SCOPE;

			for (const auto channel : std::span(anim->mChannels, anim->mNumChannels))
				if (strcmp(channel->mNodeName.C_Str(), name) == 0)
					return channel;
			return nullptr;
		}

		template<typename T>
		static unsigned int findPreviousIndex(T * arr, unsigned int size, float time) noexcept {
			KREOGL_PROFILING_SCOPE;

			for (unsigned i = 0; i < size - 1; ++i) {
				if (time < (float)arr[i + 1].mTime)
					return i;
			}
			return 0;
		}

		template<typename T, typename Func>
		static auto calculateInterpolatedValue(T * arr, unsigned int size, float time, Func func) noexcept {
			KREOGL_PROFILING_SCOPE;

			if (size == 1)
				return toglm(arr[0].mValue);

			const auto index = findPreviousIndex(arr, size, time);
			const auto & value = arr[index];
			const auto & nextValue = arr[index + 1];

			const auto deltaTime = (float)nextValue.mTime - (float)value.mTime;
			const auto factor = (time - (float)value.mTime) / (float)deltaTime;

			const auto startValue = toglm(value.mValue);
			const auto endValue = toglm(nextValue.mValue);

			return func(startValue, endValue, factor);
		}

		static glm::vec3 calculateInterpolatedPosition(const aiNodeAnim * nodeAnim, float time) noexcept {
			KREOGL_PROFILING_SCOPE;
			return calculateInterpolatedValue(nodeAnim->mPositionKeys, nodeAnim->mNumPositionKeys, time, [](const glm::vec3 & v1, const glm::vec3 & v2, float f) noexcept { return glm::mix(v1, v2, f); });
		}

		static glm::quat calculateInterpolatedRotation(const aiNodeAnim * nodeAnim, float time) noexcept {
			KREOGL_PROFILING_SCOPE;
			return calculateInterpolatedValue(nodeAnim->mRotationKeys, nodeAnim->mNumRotationKeys, time, glm::slerp<float, glm::defaultp>);
		}

		static glm::vec3 calculateInterpolatedScale(const aiNodeAnim * nodeAnim, float time) noexcept {
			KREOGL_PROFILING_SCOPE;
			return calculateInterpolatedValue(nodeAnim->mScalingKeys, nodeAnim->mNumScalingKeys, time, [](const glm::vec3 & v1, const glm::vec3 & v2, float f) noexcept { return glm::mix(v1, v2, f); });
		}
	}

	void AssImpAnimationModel::updateBoneMats(const aiNode & node, float time, AnimatedObject & object, const glm::mat4 & parentTransform, bool applyMoverBehavior) const noexcept {
		KREOGL_PROFILING_SCOPE;

		glm::mat4 totalTransform = parentTransform * toglm(node.mTransformation);
		if (const auto nodeAnim = getNodeAnim(assimpAnimation, node.mName.C_Str())) {
			glm::mat4 mat{ 1.f };

			const auto pos = calculateInterpolatedPosition(nodeAnim, time);
			const auto rot = calculateInterpolatedRotation(nodeAnim, time);
			const auto scale = calculateInterpolatedScale(nodeAnim, time);

			const auto posInWorldSpace = glm::vec3(parentTransform * glm::vec4(pos, 1.f));

			if (applyMoverBehavior) {
				applyMoverBehavior = false; // only the "root" node applies to the whole model

				auto & movementUpToLastFrame = object.animation->movementUpToLastFrame;
				switch (object.animation->positionMoverBehavior) {
					case Animation::MoverBehavior::UpdateTransform: {
						object.transform = glm::translate(object.transform, posInWorldSpace - movementUpToLastFrame.position);
						break;
					}
					case Animation::MoverBehavior::UpdateBones: {
						mat = glm::translate(mat, pos);
						break;
					}
					case Animation::MoverBehavior::None:
					default:
						break;
				}

				switch (object.animation->rotationMoverBehavior) {
					case Animation::MoverBehavior::UpdateTransform: {
						object.transform *= glm::mat4_cast(rot - movementUpToLastFrame.rotation);
						break;
					}
					case Animation::MoverBehavior::UpdateBones: {
						mat *= glm::mat4_cast(rot);
						break;
					}
					case Animation::MoverBehavior::None:
					default:
						break;
				}

				switch (object.animation->scaleMoverBehavior) {
					case Animation::MoverBehavior::UpdateTransform: {
						break;
					}
					case Animation::MoverBehavior::UpdateBones: {
						mat = glm::scale(mat, scale);
						break;
					}
					case Animation::MoverBehavior::None:
					default:
						break;
				}

				movementUpToLastFrame = {
					.position = posInWorldSpace,
					.rotation = rot,
					.scale = scale
				};
			} else {
				mat = glm::translate(mat, pos);
				mat *= glm::mat4_cast(rot);
			}
			mat = glm::scale(mat, scale);

			totalTransform = parentTransform * mat;
		}

		const auto & animatedModel = static_cast<const AnimatedModel &>(*object.model);
		object.skeleton.meshes.resize(animatedModel.skeleton->meshes.size());

		const auto & assimpSkeletonModel = static_cast<const AssImpSkeletonModel &>(*animatedModel.skeleton);
		for (unsigned int i = 0; i < animatedModel.skeleton->meshes.size(); ++i) {
			const auto & input = assimpSkeletonModel.assimpMeshes[i];
			auto & output = object.skeleton.meshes[i];

			output.boneMatsBoneSpace.resize(input.bones.size());
			output.boneMatsMeshSpace.resize(input.bones.size());

			const auto inputBone = std::ranges::find_if(input.bones, [&](const auto & bone) { return bone.node == &node; });
			if (inputBone != input.bones.end()) {
				const auto index = inputBone - input.bones.begin();
				output.boneMatsMeshSpace[index] = totalTransform;
				output.boneMatsBoneSpace[index] = totalTransform * inputBone->offset;
			}
		}

		for (const auto child : std::span(node.mChildren, node.mNumChildren))
			updateBoneMats(*child, time, object, totalTransform, applyMoverBehavior);
	}
}