#include "AssImpAnimationModel.hpp"

// glm
#include <glm/gtc/type_ptr.hpp>

// kreogl
#include "kreogl/animation/AnimatedObject.hpp"
#include "kreogl/animation/AnimatedModel.hpp"
#include "AssImpSkeletonModel.hpp"
#include "AssImpToGLM.hpp"

namespace kreogl {
    void AssImpAnimationModel::tick(float deltaTime, AnimatedObject & object) const noexcept {
        assert(assimpAnimation != nullptr);

        auto centeredTransform = object.transform;
        centeredTransform[3] = glm::vec4(0.f, 0.f, 0.f, 1.f);

        const auto & animatedModel = static_cast<const AnimatedModel &>(*object.model);
        assert(animatedModel.skeleton);
        const auto & skeletonModel = static_cast<const AssImpSkeletonModel &>(*animatedModel.skeleton);

        const UpdateBoneMatsParams params {
            .node = *skeletonModel.rootNode,
            .time = deltaTime * (float)assimpAnimation->mTicksPerSecond,
            .object = object,
            .parentTransform = glm::mat4(1.f),
            .transform = centeredTransform
        };
        updateBoneMats(params);

        incrementDeltaTime(deltaTime, object);
    }

    void AssImpAnimationModel::incrementDeltaTime(float deltaTime, AnimatedObject & object) const noexcept {
        object.animation->currentTime += deltaTime * object.animation->speed;
        const auto wrappedTime = fmodf(object.animation->currentTime, (float)assimpAnimation->mDuration);
        if (std::abs(wrappedTime - object.animation->currentTime) > 0.001f) { // We've looped
            if (object.animation->loop)
                object.animation->currentTime = wrappedTime;
            else {
                object.animation->speed = 0.f;
                object.animation->currentTime = 0.f;
            }
        }
    }

    namespace {
        static const aiNodeAnim * getNodeAnim(const aiAnimation * anim, const char * name) noexcept {
            for (size_t i = 0; i < anim->mNumChannels; ++i)
                if (strcmp(anim->mChannels[i]->mNodeName.C_Str(), name) == 0)
                    return anim->mChannels[i];
            return nullptr;
        }

        template<typename T>
        static unsigned int findPreviousIndex(T * arr, unsigned int size, float time) noexcept {
            for (unsigned i = 0; i < size - 1; ++i) {
                if (time < (float)arr[i + 1].mTime)
                    return i;
            }
            return 0;
        }

        template<typename T, typename Func>
        static auto calculateInterpolatedValue(T * arr, unsigned int size, float time, Func func) noexcept {
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
            return calculateInterpolatedValue(nodeAnim->mPositionKeys, nodeAnim->mNumPositionKeys, time, [](const glm::vec3 & v1, const glm::vec3 & v2, float f) noexcept { return glm::mix(v1, v2, f); });
        }

        static glm::quat calculateInterpolatedRotation(const aiNodeAnim * nodeAnim, float time) noexcept {
            return calculateInterpolatedValue(nodeAnim->mRotationKeys, nodeAnim->mNumRotationKeys, time, glm::slerp<float, glm::defaultp>);
        }

        static glm::vec3 calculateInterpolatedScale(const aiNodeAnim * nodeAnim, float time) noexcept {
            return calculateInterpolatedValue(nodeAnim->mScalingKeys, nodeAnim->mNumScalingKeys, time, [](const glm::vec3 & v1, const glm::vec3 & v2, float f) noexcept { return glm::mix(v1, v2, f); });
        }
    }

    void AssImpAnimationModel::updateBoneMats(const UpdateBoneMatsParams & params) const noexcept {
        glm::mat4 totalTransform = params.parentTransform * toglm(params.node.mTransformation);
        if (const auto nodeAnim = getNodeAnim(assimpAnimation, params.node.mName.C_Str())) {
            glm::mat4 mat{ 1.f };

            const auto pos = calculateInterpolatedPosition(nodeAnim, params.time);
            mat = glm::translate(mat, pos);

            const auto rot = calculateInterpolatedRotation(nodeAnim, params.time);
            mat *= glm::mat4_cast(rot);

            const auto scale = calculateInterpolatedScale(nodeAnim, params.time);
            mat *= glm::scale(mat, scale);

            totalTransform = params.parentTransform * mat;
        }

        const auto & animatedModel = static_cast<const AnimatedModel &>(*params.object.model);
        params.object.skeleton.meshes.resize(animatedModel.skeleton->meshes.size());

        const auto & assimpSkeletonModel = static_cast<const AssImpSkeletonModel &>(*animatedModel.skeleton);
        for (unsigned int i = 0; i < animatedModel.skeleton->meshes.size(); ++i) {
            const auto & input = assimpSkeletonModel.assimpMeshes[i];
            auto & output = params.object.skeleton.meshes[i];

            output.boneMatsBoneSpace.resize(input.bones.size());
            output.boneMatsMeshSpace.resize(input.bones.size());

            for (size_t boneIndex = 0; boneIndex < input.bones.size(); ++boneIndex) {
                const auto & bone = input.bones[boneIndex];
                if (bone.node == &params.node) {
                    output.boneMatsMeshSpace[boneIndex] = totalTransform;
                    output.boneMatsBoneSpace[boneIndex] = totalTransform * bone.offset;
                    break;
                }
            }
        }
    }
}