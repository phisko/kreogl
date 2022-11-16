#pragma once

#include "kreogl/impl/shaders/helpers/Skeletal/SkeletalShader.hpp"
#include "kreogl/impl/shaders/shadowMap/ShadowMapShader.hpp"
#include "kreogl/impl/shaders/Singleton.hpp"

namespace kreogl {
	class KREOGL_EXPORT SkeletalShadowMapShader : public ShadowMapShader, public SkeletalShader, public Singleton<SkeletalShadowMapShader> {
	public:
		SkeletalShadowMapShader() noexcept;

	private:
		void addSourceFiles() noexcept override;
		std::vector<UniformBase *> getUniforms() noexcept override;
		void drawToTexture(GLuint texture, const glm::mat4 & lightSpaceMatrix, const DrawParams & params) noexcept override;
	};
}