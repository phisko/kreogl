#pragma once

#include "AnimationModel.hpp"

namespace kreogl {
    struct Animation {
        const AnimationModel * model = nullptr;
        float currentTime = 0.f;
        float speed = 1.f;
        bool loop = false;
    };
}