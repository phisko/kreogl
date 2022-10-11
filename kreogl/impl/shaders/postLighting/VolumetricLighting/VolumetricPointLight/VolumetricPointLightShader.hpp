#pragma once

#include "kreogl/impl/shaders/Shader.hpp"
#include "kreogl/impl/shaders/Singleton.hpp"

#include "kreogl/impl/shaders/shadowMap/sample/SampleShadowCube/SampleShadowCubeGLSL.hpp"
#include "kreogl/impl/shaders/lighting/PointLight/PointLightGLSL.hpp"
#include "kreogl/impl/shaders/postLighting/VolumetricLighting/VolumetricLightingGLSL.hpp"

namespace kreogl {
    class VolumetricPointLightShader : public Shader, public Singleton<VolumetricPointLightShader> {
    public:
        VolumetricPointLightShader() noexcept;

    private:
        void draw(const DrawParams & params) noexcept override;
        void addSourceFiles() noexcept override;
        std::vector<UniformBase *> getUniforms() noexcept override;

    private:
        VolumetricLightingGLSL _glsl;
        PointLightGLSL::GetDirection _pointLightGLSL;
        SampleShadowCubeGLSL _shadowCubeGLSL;
    };
}