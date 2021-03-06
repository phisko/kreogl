#pragma once

#include "PointLightGLSL.hpp"
#include "kreogl/impl/shaders/helpers/PositionProjViewModel/PositionProjViewModelGLSL.hpp"
#include "kreogl/impl/shaders/shadowMap/sample/SampleShadowCube/SampleShadowCubeGLSL.hpp"

#include "kreogl/impl/shaders/Shader.hpp"
#include "kreogl/impl/shaders/Singleton.hpp"

namespace kreogl {
    class PointLightShader : public Shader, public Singleton<PointLightShader> {
    public:
        PointLightShader() noexcept;
        void draw(const DrawParams & params) noexcept override;

    private:
        void addSourceFiles() noexcept override;
        std::vector<UniformBase *> getUniforms() noexcept override;

    private:
        PositionProjViewModelGLSL _ppvmGLSL;
        PointLightGLSL _glsl;
        SampleShadowCubeGLSL _shadowCubeGLSL;
    };
}
