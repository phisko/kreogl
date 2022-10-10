#include "SkeletalShadowMapShader.hpp"

#include "kreogl/World.hpp"

namespace kreogl {
    SkeletalShadowMapShader::SkeletalShadowMapShader() noexcept {
        init("SkeletalShadowMapShader");
        useWithoutUniformCheck();
        _glsl.proj = glm::mat4(1.f);
    }

    void SkeletalShadowMapShader::addSourceFiles() noexcept {
        addSourceFile(SkeletalGLSL::vert, GL_VERTEX_SHADER);
    }

    std::vector<UniformBase *> SkeletalShadowMapShader::getUniforms() noexcept {
        return {
            &_glsl.model,
            &_glsl.view,
            &_glsl.proj,
            &_glsl.bones
        };
    }

    void SkeletalShadowMapShader::drawToTexture(GLuint texture, const glm::mat4 & lightSpaceMatrix, const DrawParams & params) noexcept {
        _glsl.view = lightSpaceMatrix;

        for (const auto object : params.world.getObjects(VertexSpecification::skeletal))
            if (object->castShadows) {
                _glsl.model = object->transform;
                drawObject(*object);
            }
    }
}