#include "SkeletalShader.hpp"
#include "kreogl/impl/shaders/helpers/ApplyTransparency/ApplyTransparencyGLSL.hpp"

// stl
#include <algorithm>

// kreogl
#include "kreogl/Camera.hpp"
#include "kreogl/World.hpp"
#include "kreogl/animation/AnimatedModel.hpp"
#include "kreogl/animation/AnimatedObject.hpp"

namespace kreogl {
    namespace {
        static constexpr auto DIFFUSE_TEXTURE_INDEX = (int)GBuffer::Texture::Count;
        static constexpr auto SPECULAR_TEXTURE_INDEX = (int)GBuffer::Texture::Count + 1;

        static void markAsUsed(UniformBase & uniform) noexcept {
#ifndef NDEBUG
            uniform.used = true;
#endif
        }
    }

    SkeletalShader::SkeletalShader() noexcept {
        init("SkeletalShader");

        useWithoutUniformCheck();
        _glsl.texture_diffuse = DIFFUSE_TEXTURE_INDEX;
        _glsl.texture_specular = SPECULAR_TEXTURE_INDEX;
    }

    void SkeletalShader::addSourceFiles() noexcept {
        addSourceFile(SkeletalGLSL::vert, GL_VERTEX_SHADER);
        addSourceFile(SkeletalGLSL::frag, GL_FRAGMENT_SHADER);
        addSourceFile(ApplyTransparencyGLSL::frag, GL_FRAGMENT_SHADER);
    }

    std::vector<UniformBase *> SkeletalShader::getUniforms() noexcept {
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

    void SkeletalShader::draw(const DrawParams & params) noexcept {
        auto uniformChecker = use(false);
        _glsl.view = params.camera.getViewMatrix();
        _glsl.proj = params.camera.getProjMatrix();

        for (const auto object : params.world.getObjects(VertexSpecification::skeletal)) {
            _glsl.model = object->transform;
            _glsl.color = object->color;
            _glsl.userData = object->userData;
            uniformChecker.shouldCheck = true;

            const auto animatedObject = static_cast<const AnimatedObject *>(object);
            const auto animatedModel = static_cast<const AnimatedModel *>(object->model);

            const bool noSkeleton = animatedObject->skeleton.meshes.empty();
            if (noSkeleton)
                uploadDefaultBones();

            for (unsigned int i = 0; i < animatedModel->meshes.size(); ++i) {
                if (!noSkeleton) {
                    const auto & bones = animatedObject->skeleton.meshes[i].boneMatsBoneSpace;
                    assert(bones.size() <= KREOGL_SKELETON_MAX_BONES);
                    glUniformMatrix4fv(_glsl.bones.location, (GLsizei)bones.size(), GL_FALSE, glm::value_ptr(bones[0]));
                    markAsUsed(_glsl.bones);
                }
                bindTextures(animatedModel->meshTextures[i]);
                animatedModel->meshes[i].draw();
            }
        }
    }

    void SkeletalShader::uploadDefaultBones() noexcept {
        static glm::mat4 defaultMats[KREOGL_SKELETON_MAX_BONES];
        static bool first = true;
        if (first) {
            std::ranges::fill(defaultMats, glm::mat4(1.f));
            first = false;
        }

        glUniformMatrix4fv(_glsl.bones.location, KREOGL_SKELETON_MAX_BONES, GL_FALSE, glm::value_ptr(defaultMats[0]));
        markAsUsed(_glsl.bones);
    }

    void SkeletalShader::bindTextures(const AnimatedModel::MeshTextures & textures) noexcept {
        // Diffuse
        const bool hasTexture = !textures.diffuseTextures.empty();
        _glsl.hasTexture = hasTexture;
        if (hasTexture) {
            glActiveTexture((GLenum)(GL_TEXTURE0 + DIFFUSE_TEXTURE_INDEX));
            glBindTexture(GL_TEXTURE_2D, *textures.diffuseTextures[0].texture);
            markAsUsed(_glsl.diffuseColor);
        }
        else
            _glsl.diffuseColor = textures.diffuseColor;

        // Specular
        if (!textures.specularTextures.empty()) {
            glActiveTexture((GLenum)(GL_TEXTURE0 + SPECULAR_TEXTURE_INDEX));
            glBindTexture(GL_TEXTURE_2D, *textures.specularTextures[0].texture);
            markAsUsed(_glsl.specularColor);
        }
        else
            _glsl.specularColor = textures.specularColor;
    }
}