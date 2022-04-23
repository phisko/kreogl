#pragma once

#include "kreogl/lights/DirectionalLight.hpp"
#include "kreogl/lights/PointLight.hpp"
#include "kreogl/lights/SpotLight.hpp"
#include "kreogl/impl/shaders/Shader.hpp"
#include "kreogl/impl/shaders/ShaderStep.hpp"

namespace kreogl {
    bool initGLFW() noexcept;
    bool initGlew() noexcept;
    void createDefaultShaders() noexcept;

    void addShader(ShaderStep step, Shader & shader) noexcept;
    void runShaders(ShaderStep step, const DrawParams & params) noexcept;
    void fillShadowMap(const DirectionalLight & light, const DrawParams & params) noexcept;
    void fillShadowMap(const SpotLight & light, const DrawParams & params) noexcept;
    void fillShadowMap(const PointLight & light, const DrawParams & params) noexcept;
}