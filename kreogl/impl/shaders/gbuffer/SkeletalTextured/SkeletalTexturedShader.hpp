#pragma once

#include "kreogl/impl/shaders/helpers/Skeletal/SkeletalShader.hpp"
#include "kreogl/impl/shaders/Shader.hpp"
#include "kreogl/impl/shaders/Singleton.hpp"

#include "kreogl/animation/AnimatedModel.hpp"

namespace kreogl {
	class KREOGL_EXPORT SkeletalTexturedShader : public Shader, public SkeletalShader, public Singleton<SkeletalTexturedShader> {
	public:
		SkeletalTexturedShader() noexcept;

	private:
		void draw(const DrawParams & params) noexcept override;
		void addSourceFiles() noexcept override;
		std::vector<UniformBase *> getUniforms() noexcept override;

	private:
		void setMeshUniforms(const AnimatedModel & model, unsigned int meshIndex) noexcept override;
		void bindTextures(const AnimatedModel::MeshTextures & textures) noexcept;
	};
}
