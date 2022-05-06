#include "kreogl.hpp"

// gl
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// stl
#include <iostream>
#include <unordered_map>
#include <vector>

// shaders
#include "kreogl/impl/shaders/gbuffer/PositionColor/PositionColorShader.hpp"

#include "kreogl/impl/shaders/lighting/DirectionalLight/DirectionalLightShader.hpp"
#include "kreogl/impl/shaders/lighting/PointLight/PointLightShader.hpp"
#include "kreogl/impl/shaders/lighting/SpotLight/SpotLightShader.hpp"

#include "kreogl/impl/shaders/shadowMap/PositionShadowCube/PositionShadowCubeShader.hpp"
#include "kreogl/impl/shaders/shadowMap/PositionShadowMap/PositionShadowMapShader.hpp"

namespace kreogl {
    struct GlobalState {
        bool isGLFWInit = false;
        bool isGlewInit = false;

        std::unordered_map<ShaderStep, std::vector<Shader *>> shadersPerStep;
    };

    static GlobalState globalState;

    bool initGLFW() noexcept {
        if (globalState.isGLFWInit)
            return true;

        glfwSetErrorCallback([](int error, const char * desc) {
            std::cerr << "GLFW error -- Code: " << error << ". Description: '" << desc << '\'' << std::endl;
        });

        if (glfwInit() != GLFW_TRUE) {
            std::cerr << "kreogl: failed to init GLFW" << std::endl;
            return false;
        }

        globalState.isGLFWInit = true;
        return true;
    }

    bool initGlew() noexcept {
        if (globalState.isGlewInit)
            return true;

        glewExperimental = true;
        if (glewInit() != GLEW_OK) {
            std::cerr << "kreogl: failed to init glew" << std::endl;
            return false;
        }

#ifndef _NDEBUG
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback([](GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar * message, const void * userParam) {
            const char * severityString;
            if (severity == GL_DEBUG_SEVERITY_MEDIUM)
                severityString = "warning";
            else if (severity == GL_DEBUG_SEVERITY_HIGH)
                severityString = "error";
            else
                return;
            std::cerr << "OpenGL " << severityString << " -- Source: " << source << ". Type: " << type << ". ID: " << id << ". Message: '" << message << '\'' << std::endl;
        }, nullptr);
#endif

        globalState.isGlewInit = true;
        return true;
    }

    void createDefaultShaders() noexcept {
        addShader(ShaderStep::GBuffer, PositionColorShader::getSingleton());
        addShader(ShaderStep::Lighting, DirectionalLightShader::getSingleton());
        addShader(ShaderStep::Lighting, PointLightShader::getSingleton());
        addShader(ShaderStep::Lighting, SpotLightShader::getSingleton());
        addShader(ShaderStep::ShadowMap, PositionShadowMapShader::getSingleton());
        addShader(ShaderStep::ShadowCube, PositionShadowCubeShader::getSingleton());
    }

    void addShader(ShaderStep step, Shader & shader) noexcept {
        globalState.shadersPerStep[step].push_back(&shader);
    }

    void runShaders(ShaderStep step, const DrawParams & params) noexcept {
        for (auto shader : globalState.shadersPerStep[step])
            shader->draw(params);
    }

    void fillShadowMap(const DirectionalLight & light, const DrawParams & params) noexcept {
        const auto & shaders = globalState.shadersPerStep[ShaderStep::ShadowMap];
        for (const auto shader : shaders) {
            const auto shadowMapShader = static_cast<ShadowMapShader *>(shader);
            shadowMapShader->draw(light, params);
        }
    }

    void fillShadowMap(const SpotLight & light, const DrawParams & params) noexcept {
        const auto & shaders = globalState.shadersPerStep[ShaderStep::ShadowMap];
        for (const auto shader : shaders) {
            const auto shadowMapShader = static_cast<ShadowMapShader *>(shader);
            shadowMapShader->draw(light, params);
        }
    }

    void fillShadowMap(const PointLight & light, const DrawParams & params) noexcept {
        const auto & shaders = globalState.shadersPerStep[ShaderStep::ShadowCube];
        for (const auto shader : shaders) {
            const auto shadowCubeShader = static_cast<ShadowCubeShader *>(shader);
            shadowCubeShader->draw(light, params);
        }
    }
}