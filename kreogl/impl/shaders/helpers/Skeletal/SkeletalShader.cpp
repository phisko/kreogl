#include "SkeletalShader.hpp"

// stl
#include <algorithm>

// kreogl
#include "kreogl/animation/AnimatedObject.hpp"
#include "kreogl/impl/kreogl_profiling.hpp"

namespace kreogl {
	void SkeletalShader::uploadDefaultBones() noexcept {
		KREOGL_PROFILING_SCOPE;

		static glm::mat4 defaultMats[KREOGL_SKELETON_MAX_BONES];
		static bool first = true;
		if (first) {
			std::ranges::fill(defaultMats, glm::mat4(1.f));
			first = false;
		}

		glUniformMatrix4fv(_glsl.bones.location, KREOGL_SKELETON_MAX_BONES, GL_FALSE, glm::value_ptr(defaultMats[0]));
		_glsl.bones.markAsUsed();
	}

	void SkeletalShader::drawObject(const Object & object) noexcept {
		KREOGL_PROFILING_SCOPE;

		const auto animatedObject = static_cast<const AnimatedObject &>(object);
		const auto animatedModel = static_cast<const AnimatedModel *>(object.model);

		const bool noSkeleton = animatedObject.skeleton.meshes.empty();
		if (noSkeleton)
			uploadDefaultBones();

		for (unsigned int i = 0; i < animatedModel->meshes.size(); ++i) {
			if (!noSkeleton) {
				const auto & bones = animatedObject.skeleton.meshes[i].boneMatsBoneSpace;
				assert(bones.size() <= KREOGL_SKELETON_MAX_BONES);
				glUniformMatrix4fv(_glsl.bones.location, (GLsizei)bones.size(), GL_FALSE, glm::value_ptr(bones[0]));
				_glsl.bones.markAsUsed();
			}
			setMeshUniforms(*animatedModel, i);
			animatedModel->meshes[i].draw();
		}
	}
}