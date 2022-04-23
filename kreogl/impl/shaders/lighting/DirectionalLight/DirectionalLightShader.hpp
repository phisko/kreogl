#pragma once

#include "DirectionalLightGLSL.hpp"
#include "kreogl/impl/shaders/shadowMap/CascadedShadowMap/CascadedShadowMapGLSL.hpp"
#include "kreogl/impl/shaders/Shader.hpp"
#include "kreogl/impl/shaders/Singleton.hpp"
#include "kreogl/lights/DirectionalLight.hpp"

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
        CascadedShadowMapGLSL _csmGLSL;
    };
}