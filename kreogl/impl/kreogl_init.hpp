#pragma once

#include "kreogl/lights/DirectionalLight.hpp"
#include "kreogl/lights/PointLight.hpp"
#include "kreogl/lights/SpotLight.hpp"
#include "kreogl/impl/shaders/Shader.hpp"
#include "kreogl/impl/shaders/ShaderStep.hpp"

namespace kreogl {
    bool initGLFW() noexcept;
    bool initGlew() noexcept;
}