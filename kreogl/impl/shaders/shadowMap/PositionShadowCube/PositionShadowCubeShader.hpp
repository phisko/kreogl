#pragma once

#include "kreogl/impl/shaders/Singleton.hpp"
#include "kreogl/impl/shaders/shadowMap/ShadowCubeShader.hpp"
#include "kreogl/impl/shaders/helpers/PositionProjViewModel/PositionProjViewModelGLSL.hpp"

namespace kreogl {
    class PositionShadowCubeShader : public ShadowCubeShader, public Singleton<PositionShadowCubeShader> {
    public:
        PositionShadowCubeShader() noexcept;
        void drawObjects(const DrawParams & params) noexcept override;

    private:
        void addSourceFiles() noexcept override;
        std::vector<UniformBase *> getUniforms() noexcept override;

    private:
        PositionProjViewModelGLSL _glsl;
    };
}