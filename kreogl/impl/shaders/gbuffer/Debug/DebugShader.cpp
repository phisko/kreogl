#include "DebugShader.hpp"
#include "kreogl/impl/shaders/helpers/ApplyTransparency/ApplyTransparencyGLSL.hpp"

#include "kreogl/Camera.hpp"
#include "kreogl/World.hpp"
#include "kreogl/impl/shaders/VertexSpecification.hpp"
#include "kreogl/impl/shapes/Line.hpp"
#include "kreogl/impl/shapes/Sphere.hpp"
#include "kreogl/impl/shapes/Box.hpp"
#include "kreogl/impl/kreogl_profiling.hpp"

namespace kreogl {
    DebugShader::DebugShader() noexcept {
        KREOGL_PROFILING_SCOPE;

        init("DebugShader");
    }

    void DebugShader::addSourceFiles() noexcept {
        KREOGL_PROFILING_SCOPE;

        addSourceFile(DebugGLSL::vert, GL_VERTEX_SHADER);
        addSourceFile(DebugGLSL::frag, GL_FRAGMENT_SHADER);
        addSourceFile(ApplyTransparencyGLSL::frag, GL_FRAGMENT_SHADER);
    }

    std::vector<UniformBase *> DebugShader::getUniforms() noexcept {
        KREOGL_PROFILING_SCOPE;

        return {
            &_glsl.proj,
            &_glsl.view,
            &_glsl.model,
            &_glsl.viewPos,
            &_glsl.color,
            &_glsl.userData
        };
    }

    void DebugShader::draw(const DrawParams & params) noexcept {
        KREOGL_PROFILING_SCOPE;

        auto uniformChecker = use(false);

        _glsl.view = params.camera.getViewMatrix();
        _glsl.proj = params.camera.getProjMatrix();
        _glsl.viewPos = params.camera.getPosition();

        for (const auto element : params.world.getDebugElements()) {
            uniformChecker.shouldCheck = true;

            _glsl.model = element->transform;
            _glsl.color = element->color;
            _glsl.userData = element->userData;

            switch (element->type) {
                case DebugElement::Type::Line:
                    kreogl::shapes::drawLine(glm::vec3{ 0.f }, element->lineEnd);
                    break;
                case DebugElement::Type::Sphere:
                    kreogl::shapes::drawSphere();
                    break;
                case DebugElement::Type::Box:
                    kreogl::shapes::drawBox();
                    break;
                default:
                    assert(false); // Non-exhaustive switch
                    break;
            }
        }
    }
}