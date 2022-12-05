#include "skeletal_shader.hpp"

// stl
#include <algorithm>

// kreogl
#include "kreogl/animation/animated_object.hpp"
#include "kreogl/impl/kreogl_profiling.hpp"

namespace kreogl {
	void skeletal_shader::upload_default_bones() noexcept {
		KREOGL_PROFILING_SCOPE;

		static glm::mat4 default_mats[KREOGL_SKELETON_MAX_BONES];
		static bool first = true;
		if (first) {
			std::ranges::fill(default_mats, glm::mat4(1.f));
			first = false;
		}

		glUniformMatrix4fv(_glsl.bones.location, KREOGL_SKELETON_MAX_BONES, GL_FALSE, glm::value_ptr(default_mats[0]));
		_glsl.bones.mark_as_used();
	}

	void skeletal_shader::draw_object(const object & object) noexcept {
		KREOGL_PROFILING_SCOPE;

		const auto animated_object = static_cast<const kreogl::animated_object &>(object);
		const auto animated_model = static_cast<const kreogl::animated_model *>(object.model);

		const bool no_skeleton = animated_object.skeleton.meshes.empty();
		if (no_skeleton)
			upload_default_bones();

		for (unsigned int i = 0; i < animated_model->meshes.size(); ++i) {
			if (!no_skeleton) {
				const auto & bones = animated_object.skeleton.meshes[i].bone_mats_bone_space;
				assert(bones.size() <= KREOGL_SKELETON_MAX_BONES);
				glUniformMatrix4fv(_glsl.bones.location, (GLsizei)bones.size(), GL_FALSE, glm::value_ptr(bones[0]));
				_glsl.bones.mark_as_used();
			}
			set_mesh_uniforms(*animated_model, i);
			animated_model->meshes[i].draw();
		}
	}
}