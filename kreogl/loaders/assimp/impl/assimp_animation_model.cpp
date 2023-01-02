#include "assimp_animation_model.hpp"

// stl
#include <span>

// glm
#include <glm/gtc/type_ptr.hpp>

// kreogl
#include "kreogl/animation/animated_object.hpp"
#include "kreogl/animation/animated_model.hpp"
#include "kreogl/impl/kreogl_profiling.hpp"
#include "assimp_skeleton_model.hpp"
#include "assimp_to_glm.hpp"

namespace kreogl {
	void assimp_animation_model::tick(float delta_time, animated_object & object) const noexcept {
		KREOGL_PROFILING_SCOPE;

		assert(assimp_animation != nullptr);

		const auto & animated_model = static_cast<const kreogl::animated_model &>(*object.model);
		assert(animated_model.skeleton);

		const auto & skeleton_model = static_cast<const kreogl::assimp_skeleton_model &>(*animated_model.skeleton);
		update_bone_mats(*skeleton_model.root_node, object.animation->current_time * ticks_per_second, object, glm::mat4{ 1.f }, true);
		increment_delta_time(delta_time, object);
	}

	void assimp_animation_model::increment_delta_time(float delta_time, animated_object & object) const noexcept {
		KREOGL_PROFILING_SCOPE;

		object.animation->current_time += delta_time * object.animation->speed;
		const auto wrapped_time = fmodf(object.animation->current_time, total_time);
		if (wrapped_time < object.animation->current_time) { // We've looped
			if (object.animation->loop)
				object.animation->current_time = wrapped_time;
			else
				object.animation = std::nullopt;
		}
	}

	namespace {
		static const aiNodeAnim * get_node_anim(const aiAnimation * anim, const char * name) noexcept {
			KREOGL_PROFILING_SCOPE;

			for (const auto channel : std::span(anim->mChannels, anim->mNumChannels))
				if (strcmp(channel->mNodeName.C_Str(), name) == 0)
					return channel;
			return nullptr;
		}

		template<typename T>
		static unsigned int find_previous_index(T * arr, unsigned int size, float time) noexcept {
			KREOGL_PROFILING_SCOPE;

			for (unsigned i = 0; i < size - 1; ++i) {
				if (time < (float)arr[i + 1].mTime)
					return i;
			}
			return 0;
		}

		template<typename T, typename Func>
		static auto calculate_interpolated_value(T * arr, unsigned int size, float time, Func func) noexcept {
			KREOGL_PROFILING_SCOPE;

			if (size == 1)
				return toglm(arr[0].mValue);

			const auto index = find_previous_index(arr, size, time);
			const auto & value = arr[index];
			const auto & next_value = arr[index + 1];

			const auto delta_time = (float)next_value.mTime - (float)value.mTime;
			const auto factor = (time - (float)value.mTime) / (float)delta_time;

			const auto start_value = toglm(value.mValue);
			const auto end_value = toglm(next_value.mValue);

			return func(start_value, end_value, factor);
		}

		static glm::vec3 calculate_interpolated_position(const aiNodeAnim * node_anim, float time) noexcept {
			KREOGL_PROFILING_SCOPE;
			return calculate_interpolated_value(node_anim->mPositionKeys, node_anim->mNumPositionKeys, time, [](const glm::vec3 & v1, const glm::vec3 & v2, float f) noexcept { return glm::mix(v1, v2, f); });
		}

		static glm::quat calculate_interpolated_rotation(const aiNodeAnim * node_anim, float time) noexcept {
			KREOGL_PROFILING_SCOPE;
			return calculate_interpolated_value(node_anim->mRotationKeys, node_anim->mNumRotationKeys, time, glm::slerp<float, glm::defaultp>);
		}

		static glm::vec3 calculate_interpolated_scale(const aiNodeAnim * node_anim, float time) noexcept {
			KREOGL_PROFILING_SCOPE;
			return calculate_interpolated_value(node_anim->mScalingKeys, node_anim->mNumScalingKeys, time, [](const glm::vec3 & v1, const glm::vec3 & v2, float f) noexcept { return glm::mix(v1, v2, f); });
		}
	}

	void assimp_animation_model::update_bone_mats(const aiNode & node, float time, animated_object & object, const glm::mat4 & parent_transform, bool apply_mover_behavior) const noexcept {
		KREOGL_PROFILING_SCOPE;

		glm::mat4 total_transform = parent_transform * toglm(node.mTransformation);
		if (const auto node_anim = get_node_anim(assimp_animation, node.mName.C_Str())) {
			glm::mat4 mat{ 1.f };

			const auto pos = calculate_interpolated_position(node_anim, time);
			const auto rot = calculate_interpolated_rotation(node_anim, time);
			const auto scale = calculate_interpolated_scale(node_anim, time);

			const auto pos_in_world_space = glm::vec3(parent_transform * glm::vec4(pos, 1.f));

			if (apply_mover_behavior) {
				apply_mover_behavior = false; // only the "root" node applies to the whole model

				auto & movement_up_to_last_frame = object.animation->movement_up_to_last_frame;
				switch (object.animation->position_mover_behavior) {
					case animation::mover_behavior::update_transform: {
						object.transform = glm::translate(object.transform, pos_in_world_space - movement_up_to_last_frame.position);
						break;
					}
					case animation::mover_behavior::update_bones: {
						mat = glm::translate(mat, pos);
						break;
					}
					case animation::mover_behavior::none:
					default:
						break;
				}

				switch (object.animation->rotation_mover_behavior) {
					case animation::mover_behavior::update_transform: {
						object.transform *= glm::mat4_cast(rot - movement_up_to_last_frame.rotation);
						break;
					}
					case animation::mover_behavior::update_bones: {
						mat *= glm::mat4_cast(rot);
						break;
					}
					case animation::mover_behavior::none:
					default:
						break;
				}

				switch (object.animation->scale_mover_behavior) {
					case animation::mover_behavior::update_transform: {
						break;
					}
					case animation::mover_behavior::update_bones: {
						mat = glm::scale(mat, scale);
						break;
					}
					case animation::mover_behavior::none:
					default:
						break;
				}

				movement_up_to_last_frame = {
					.position = pos_in_world_space,
					.rotation = rot,
					.scale = scale
				};
			}
			else {
				mat = glm::translate(mat, pos);
				mat *= glm::mat4_cast(rot);
			}
			mat = glm::scale(mat, scale);

			total_transform = parent_transform * mat;
		}

		const auto & animated_model = static_cast<const kreogl::animated_model &>(*object.model);
		object.skeleton.meshes.resize(animated_model.skeleton->meshes.size());

		const auto & assimp_skeleton_model = static_cast<const kreogl::assimp_skeleton_model &>(*animated_model.skeleton);
		for (unsigned int i = 0; i < animated_model.skeleton->meshes.size(); ++i) {
			const auto & input = assimp_skeleton_model.assimp_meshes[i];
			auto & output = object.skeleton.meshes[i];

			output.bone_mats_bone_space.resize(input.bones.size());
			output.bone_mats_mesh_space.resize(input.bones.size());

			const auto input_bone = std::ranges::find_if(input.bones, [&](const auto & bone) { return bone.node == &node; });
			if (input_bone != input.bones.end()) {
				const auto index = input_bone - input.bones.begin();
				output.bone_mats_mesh_space[index] = total_transform;
				output.bone_mats_bone_space[index] = total_transform * input_bone->offset;
			}
		}

		for (const auto child : std::span(node.mChildren, node.mNumChildren))
			update_bone_mats(*child, time, object, total_transform, apply_mover_behavior);
	}
}