#pragma once

#include "kreogl/impl/shaders/Shader.hpp"
#include "kreogl/impl/shaders/Singleton.hpp"

#include "DirectionalLightGLSL.hpp"
#include "kreogl/impl/shaders/shadowMap/sample/SampleCascadedShadowMap/SampleCascadedShadowMapGLSL.hpp"

namespace kreogl {
    class DirectionalLightShader : public Shader, public Singleton<DirectionalLightShader> {
    public:
        DirectionalLightShader() noexcept;
        void draw(const DrawParams & params) noexcept override;

    private:
        void addSourceFiles() noexcept override;
        std::vector<UniformBase *> getUniforms() noexcept override;

    private:
        DirectionalLightGLSL _glsl;
        SampleCascadedShadowMapGLSL _csmGLSL;
    };
}