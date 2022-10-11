#pragma once

namespace kreogl {
    struct DrawParams {
        const class World & world;
        const class Camera & camera;
        const class ShaderPipeline & shaderPipeline;
    };
}