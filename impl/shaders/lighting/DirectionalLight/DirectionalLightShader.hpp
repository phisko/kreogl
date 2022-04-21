#pragma once

#include "DirectionalLightGLSL.hpp"
#include "impl/shaders/CascadedShadowMap/CascadedShadowMapGLSL.hpp"
#include "impl/shaders/Shader.hpp"
#include "impl/shaders/Singleton.hpp"
#include "lights/DirectionalLight.hpp"

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