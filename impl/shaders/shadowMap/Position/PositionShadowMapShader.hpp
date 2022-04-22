#pragma once

#include "impl/shaders/helpers/PositionProjViewModel/PositionProjViewModelGLSL.hpp"
#include "impl/shaders/ShadowMapShader.hpp"
#include "impl/shaders/Singleton.hpp"

namespace kreogl {
    class PositionShadowMapShader : public ShadowMapShader, public Singleton<PositionShadowMapShader> {
	public:
        PositionShadowMapShader() noexcept;
        void drawToTexture(GLuint texture, const glm::mat4 & lightSpaceMatrix, const DrawParams & params) noexcept override;

    private:
        void addSourceFiles() noexcept override;
        std::vector<UniformBase *> getUniforms() noexcept override;

    private:
        PositionProjViewModelGLSL _glsl;
    };
}