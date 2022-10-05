#pragma once

#include "SkeletalGLSL.hpp"
#include "kreogl/impl/shaders/Shader.hpp"
#include "kreogl/impl/shaders/Singleton.hpp"

#include "kreogl/animation/AnimatedModel.hpp"

namespace kreogl {
    class SkeletalShader : public Shader, public Singleton<SkeletalShader> {
    public:
        SkeletalShader() noexcept;
        void draw(const DrawParams & params) noexcept override;

    private:
        void addSourceFiles() noexcept override;
        std::vector<UniformBase *> getUniforms() noexcept override;

    private:
        void uploadDefaultBones() noexcept;
        void bindTextures(const AnimatedModel::MeshTextures & textures) noexcept;

    private:
        SkeletalGLSL _glsl;
    };
}
