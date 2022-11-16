#pragma once

#include "kreogl/impl/shaders/helpers/Skeletal/SkeletalShader.hpp"
#include "kreogl/impl/shaders/shadowMap/ShadowCubeShader.hpp"
#include "kreogl/impl/shaders/Singleton.hpp"

namespace kreogl {
	class KREOGL_EXPORT SkeletalShadowCubeShader : public ShadowCubeShader, public SkeletalShader, public Singleton<SkeletalShadowCubeShader> {
	public:
		SkeletalShadowCubeShader() noexcept;

	private:
		void addSourceFiles() noexcept override;
		std::vector<UniformBase *> getUniforms() noexcept override;
		void drawObjects(const DrawParams & params) noexcept override;
	};
}