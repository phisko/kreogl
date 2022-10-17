#include "SpriteShader.hpp"
#include "kreogl/impl/shaders/helpers/ApplyTransparency/ApplyTransparencyGLSL.hpp"

#include "kreogl/Camera.hpp"
#include "kreogl/World.hpp"
#include "kreogl/shapes/TexturedQuad.hpp"

namespace kreogl {
    SpriteShader::SpriteShader() noexcept {
        init("SpriteShader");

        useWithoutUniformCheck();
        _glsl.tex = (int)GBuffer::Texture::Count;
    }

    void SpriteShader::addSourceFiles() noexcept {
        addSourceFile(SpriteGLSL::vert, GL_VERTEX_SHADER);
        addSourceFile(SpriteGLSL::frag, GL_FRAGMENT_SHADER);
        addSourceFile(ApplyTransparencyGLSL::frag, GL_FRAGMENT_SHADER);
    }

    std::vector<UniformBase *> SpriteShader::getUniforms() noexcept {
        return {
            &_glsl.proj,
            &_glsl.view,
            &_glsl.model,
            &_glsl.viewPos,
            &_glsl.tex,
            &_glsl.color,
            &_glsl.userData
        };
    }

    void SpriteShader::draw(const DrawParams & params) noexcept {
        auto uniformChecker = use(false);

        _glsl.viewPos = params.camera.getPosition();
        glActiveTexture((GLenum)(GL_TEXTURE0 + (int)GBuffer::Texture::Count));

        _glsl.view = glm::mat4{ 1.f };
        _glsl.proj = glm::mat4{ 1.f };
        for (const auto sprite : params.world.getSprite2Ds()) {
            uniformChecker.shouldCheck = true;
            draw(params, *sprite);
        }

        _glsl.view = params.camera.getViewMatrix();
        _glsl.proj = params.camera.getProjMatrix();
        for (const auto sprite : params.world.getSprite3Ds()) {
            uniformChecker.shouldCheck = true;
            draw(params, *sprite);
        }
    }

    void SpriteShader::draw(const DrawParams & params, const Sprite & sprite) noexcept {
        _glsl.model = sprite.transform;
        _glsl.color = sprite.color;
        _glsl.userData = sprite.userData;

        glBindTexture(GL_TEXTURE_2D, *sprite.texture->texture);
        shapes::drawTexturedQuad();
    }
}