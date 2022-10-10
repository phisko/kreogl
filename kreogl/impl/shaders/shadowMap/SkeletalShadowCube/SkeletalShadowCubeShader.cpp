#include "SkeletalShadowCubeShader.hpp"
#include "kreogl/impl/shaders/VertexSpecification.hpp"
#include "kreogl/World.hpp"

namespace kreogl {
    SkeletalShadowCubeShader::SkeletalShadowCubeShader() noexcept {
        init("SkeletalShadowCubeShader");
        useWithoutUniformCheck();
        SkeletalShader::_glsl.proj = glm::mat4(1.f);
        SkeletalShader::_glsl.view = glm::mat4(1.f);
    }

    void SkeletalShadowCubeShader::addSourceFiles() noexcept {
        ShadowCubeShader::addSourceFiles();
        addSourceFile(SkeletalGLSL::vert, GL_VERTEX_SHADER);
    }

    std::vector<UniformBase *> SkeletalShadowCubeShader::getUniforms() noexcept {
        auto uniforms = ShadowCubeShader::getUniforms();
        uniforms.push_back(&SkeletalShader::_glsl.proj);
        uniforms.push_back(&SkeletalShader::_glsl.view);
        uniforms.push_back(&SkeletalShader::_glsl.model);
        uniforms.push_back(&SkeletalShader::_glsl.bones);
        return uniforms;
    }

    void SkeletalShadowCubeShader::drawObjects(const DrawParams & params) noexcept {
        for (const auto object : params.world.getObjects(VertexSpecification::skeletal))
            if (object->castShadows) {
                SkeletalShader::_glsl.model = object->transform;
                drawObject(*object);
            }
    }
}