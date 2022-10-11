#pragma once

#include "kreogl/impl/shaders/Shader.hpp"
#include "kreogl/impl/shaders/Singleton.hpp"
#include "kreogl/lights/SpotLight.hpp"

#include "SpotLightGLSL.hpp"
#include "kreogl/impl/shaders/helpers/PositionProjViewModel/PositionProjViewModelGLSL.hpp"
#include "kreogl/impl/shaders/shadowMap/sample/SampleShadowMap/SampleShadowMapGLSL.hpp"

namespace kreogl {
    class SpotLightShader : public Shader, public Singleton<SpotLightShader> {
    public:
        SpotLightShader() noexcept;

    private:
        void draw(const DrawParams & params) noexcept override;
        void addSourceFiles() noexcept override;
        std::vector<UniformBase *> getUniforms() noexcept override;

    private:
        SpotLightGLSL _glsl;
        PositionProjViewModelGLSL _ppvmGLSL;
        SampleShadowMapGLSL _shadowMapGLSL;
    };
}