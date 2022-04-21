#pragma once

#include "lights/DirectionalLight.hpp"
#include "lights/PointLight.hpp"
#include "lights/SpotLight.hpp"
#include "shaders/Shader.hpp"
#include "shaders/ShaderStep.hpp"

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