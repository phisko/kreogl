#include "ShaderPipeline.hpp"

#ifdef KREOGL_DEFAULT_SHADERS
// lighting
# include "kreogl/impl/shaders/lighting/DirectionalLight/DirectionalLightShader.hpp"
# include "kreogl/impl/shaders/lighting/PointLight/PointLightShader.hpp"
# include "kreogl/impl/shaders/lighting/SpotLight/SpotLightShader.hpp"

// volumetric lighting
# include "kreogl/impl/shaders/postLighting/VolumetricLighting/VolumetricDirectionalLight/VolumetricDirectionalLightShader.hpp"
# include "kreogl/impl/shaders/postLighting/VolumetricLighting/VolumetricPointLight/VolumetricPointLightShader.hpp"
# include "kreogl/impl/shaders/postLighting/VolumetricLighting/VolumetricSpotLight/VolumetricSpotLightShader.hpp"

// voxels
# include "kreogl/impl/shaders/gbuffer/PositionColor/PositionColorShader.hpp"
# include "kreogl/impl/shaders/shadowMap/PositionShadowCube/PositionShadowCubeShader.hpp"
# include "kreogl/impl/shaders/shadowMap/PositionShadowMap/PositionShadowMapShader.hpp"

// skeletal
# include "kreogl/impl/shaders/gbuffer/SkeletalTextured/SkeletalTexturedShader.hpp"
# include "kreogl/impl/shaders/shadowMap/SkeletalShadowMap/SkeletalShadowMapShader.hpp"
# include "kreogl/impl/shaders/shadowMap/SkeletalShadowCube/SkeletalShadowCubeShader.hpp"
#endif

namespace kreogl {
#ifdef KREOGL_DEFAULT_SHADERS
    const ShaderPipeline & ShaderPipeline::getDefaultShaders() noexcept {
        static const ShaderPipeline defaultShaders = []() noexcept {
            ShaderPipeline pipeline;

            // Lighting
            pipeline.addShader(ShaderStep::Lighting, DirectionalLightShader::getSingleton());
            pipeline.addShader(ShaderStep::Lighting, PointLightShader::getSingleton());
            pipeline.addShader(ShaderStep::Lighting, SpotLightShader::getSingleton());

            // Volumetric
            pipeline.addShader(ShaderStep::PostLighting, VolumetricDirectionalLightShader::getSingleton());
            pipeline.addShader(ShaderStep::PostLighting, VolumetricPointLightShader::getSingleton());
            pipeline.addShader(ShaderStep::PostLighting, VolumetricSpotLightShader::getSingleton());

            // Voxels
            pipeline.addShader(ShaderStep::GBuffer, PositionColorShader::getSingleton());
            pipeline.addShader(ShaderStep::ShadowMap, PositionShadowMapShader::getSingleton());
            pipeline.addShader(ShaderStep::ShadowCube, PositionShadowCubeShader::getSingleton());

            // Skeletal meshes
            pipeline.addShader(ShaderStep::GBuffer, SkeletalTexturedShader::getSingleton());
            pipeline.addShader(ShaderStep::ShadowMap, SkeletalShadowMapShader::getSingleton());
            pipeline.addShader(ShaderStep::ShadowCube, SkeletalShadowCubeShader::getSingleton());

            return pipeline;
        }();

        return defaultShaders;
    }
#endif

    void ShaderPipeline::addShader(ShaderStep step, Shader & shader) noexcept {
        _shadersPerStep[step].push_back(&shader);
    }

    void ShaderPipeline::runShaders(ShaderStep step, const DrawParams & params) const noexcept {
        const auto it = _shadersPerStep.find(step);
        if (it == _shadersPerStep.end())
            return;

        const auto & shaders = it->second;
        for (const auto shader : shaders)
            shader->draw(params);
    }

    const std::vector<Shader *> * ShaderPipeline::getShaders(ShaderStep step) const noexcept {
        const auto it = _shadersPerStep.find(step);
        if (it == _shadersPerStep.end())
            return nullptr;
        return &it->second;
    }
}