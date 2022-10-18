#include "ShaderPipeline.hpp"

// stl
#include <algorithm>

// kreogl
#include "kreogl/impl/kreogl_profiling.hpp"

// shaders
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

// misc
# include "kreogl/impl/shaders/gbuffer/Debug/DebugShader.hpp"
# include "kreogl/impl/shaders/gbuffer/Sprite/SpriteShader.hpp"
# include "kreogl/impl/shaders/gbuffer/Text/TextShader.hpp"
# include "kreogl/impl/shaders/postProcess/LightSphere/LightSphereShader.hpp"
# include "kreogl/impl/shaders/postProcess/Skybox/SkyboxShader.hpp"
#endif

namespace kreogl {
#ifdef KREOGL_DEFAULT_SHADERS
    const ShaderPipeline & ShaderPipeline::getDefaultShaders() noexcept {
        static const ShaderPipeline defaultShaders = []() noexcept {
            KREOGL_PROFILING_SCOPE;

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

            // Misc
            pipeline.addShader(ShaderStep::GBuffer, DebugShader::getSingleton());
            pipeline.addShader(ShaderStep::GBuffer, SpriteShader::getSingleton());
            pipeline.addShader(ShaderStep::GBuffer, TextShader::getSingleton());
            pipeline.addShader(ShaderStep::PostProcess, LightSphereShader::getSingleton());
            pipeline.addShader(ShaderStep::PostProcess, SkyboxShader::getSingleton());

            return pipeline;
        }();

        return defaultShaders;
    }
#endif

    void ShaderPipeline::addShader(ShaderStep step, Shader & shader) noexcept {
        KREOGL_PROFILING_SCOPE;

        _shadersPerStep[step].push_back(&shader);
    }

    void ShaderPipeline::removeShader(ShaderStep step, const Shader & shader) noexcept {
        KREOGL_PROFILING_SCOPE;

        const auto shaders = _shadersPerStep.find(step);
        if (shaders == _shadersPerStep.end())
            return;
        const auto it = std::ranges::find(shaders->second, &shader);
        if (it == shaders->second.end())
            return;
        shaders->second.erase(it);
    }

    void ShaderPipeline::runShaders(ShaderStep step, const DrawParams & params) const noexcept {
        KREOGL_PROFILING_SCOPE;

        const auto it = _shadersPerStep.find(step);
        if (it == _shadersPerStep.end())
            return;

        const auto & shaders = it->second;
        for (const auto shader : shaders)
            shader->draw(params);
    }

    const std::vector<Shader *> * ShaderPipeline::getShaders(ShaderStep step) const noexcept {
        KREOGL_PROFILING_SCOPE;

        const auto it = _shadersPerStep.find(step);
        if (it == _shadersPerStep.end())
            return nullptr;
        return &it->second;
    }
}