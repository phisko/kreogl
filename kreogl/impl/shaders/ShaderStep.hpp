#pragma once

namespace kreogl {
    enum class ShaderStep {
        // deferred
        GBuffer,
        Lighting,
        PostLighting,
        PostProcess,
        // shadow maps
        ShadowMap,
        ShadowCube
    };
}