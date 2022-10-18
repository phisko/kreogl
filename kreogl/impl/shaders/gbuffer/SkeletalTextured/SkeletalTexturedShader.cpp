#include "SkeletalTexturedShader.hpp"

// kreogl
#include "kreogl/impl/shaders/helpers/ApplyTransparency/ApplyTransparencyGLSL.hpp"
#include "kreogl/Camera.hpp"
#include "kreogl/World.hpp"
#include "kreogl/impl/kreogl_profiling.hpp"

namespace kreogl {
    namespace {
        static constexpr auto DIFFUSE_TEXTURE_INDEX = (int)GBuffer::Texture::Count;
        static constexpr auto SPECULAR_TEXTURE_INDEX = (int)GBuffer::Texture::Count + 1;
    }

    SkeletalTexturedShader::SkeletalTexturedShader() noexcept {
        KREOGL_PROFILING_SCOPE;

        init("SkeletalTexturedShader");

        useWithoutUniformCheck();
        _glsl.texture_diffuse = DIFFUSE_TEXTURE_INDEX;
        _glsl.texture_specular = SPECULAR_TEXTURE_INDEX;
    }

    void SkeletalTexturedShader::addSourceFiles() noexcept {
        KREOGL_PROFILING_SCOPE;

        addSourceFile(SkeletalGLSL::vert, GL_VERTEX_SHADER);
        addSourceFile(SkeletalGLSL::frag, GL_FRAGMENT_SHADER);
        addSourceFile(ApplyTransparencyGLSL::frag, GL_FRAGMENT_SHADER);
    }

    std::vector<UniformBase *> SkeletalTexturedShader::getUniforms() noexcept {
        KREOGL_PROFILING_SCOPE;

         return {
            &_glsl.model,
            &_glsl.view,
            &_glsl.proj,
            &_glsl.bones,
            &_glsl.hasTexture,
            &_glsl.texture_diffuse,
            &_glsl.texture_specular,
            &_glsl.diffuseColor,
            &_glsl.specularColor,
            &_glsl.color,
            &_glsl.userData
        };
    }

    void SkeletalTexturedShader::draw(const DrawParams & params) noexcept {
        KREOGL_PROFILING_SCOPE;

        auto uniformChecker = use(false);
        _glsl.view = params.camera.getViewMatrix();
        _glsl.proj = params.camera.getProjMatrix();

        for (const auto object : params.world.getObjects(VertexSpecification::skeletal)) {
            _glsl.model = object->transform;
            _glsl.color = object->color;
            _glsl.userData = object->userData;
            uniformChecker.shouldCheck = true;

            drawObject(*object);
        }
    }

    void SkeletalTexturedShader::setMeshUniforms(const AnimatedModel & model, unsigned int meshIndex) noexcept {
        KREOGL_PROFILING_SCOPE;

        bindTextures(model.meshTextures[meshIndex]);
    }

    void SkeletalTexturedShader::bindTextures(const AnimatedModel::MeshTextures & textures) noexcept {
        KREOGL_PROFILING_SCOPE;

        // Diffuse
        const bool hasTexture = !textures.diffuseTextures.empty();
        _glsl.hasTexture = hasTexture;
        if (hasTexture) {
            glActiveTexture((GLenum)(GL_TEXTURE0 + DIFFUSE_TEXTURE_INDEX));
            glBindTexture(GL_TEXTURE_2D, *textures.diffuseTextures[0].texture);
            _glsl.diffuseColor.markAsUsed();
        }
        else
            _glsl.diffuseColor = textures.diffuseColor;

        // Specular
        if (!textures.specularTextures.empty()) {
            glActiveTexture((GLenum)(GL_TEXTURE0 + SPECULAR_TEXTURE_INDEX));
            glBindTexture(GL_TEXTURE_2D, *textures.specularTextures[0].texture);
            _glsl.specularColor.markAsUsed();
        }
        else
            _glsl.specularColor = textures.specularColor;
    }
}