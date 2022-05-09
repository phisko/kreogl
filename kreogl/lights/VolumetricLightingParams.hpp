#pragma once

namespace kreogl {
    struct VolumetricLightingParams {
        float scattering = .1f;
        float nbSteps = 10.f;
        float defaultStepLength = 25.f;
        float intensity = 2.5f;
    };
}