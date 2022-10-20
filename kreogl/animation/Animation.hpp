#pragma once

#include <glm/glm.hpp>
#include <glm/detail/type_quat.hpp>
#include "AnimationModel.hpp"

namespace kreogl {
    struct Animation {
        const AnimationModel * model = nullptr;
        float currentTime = 0.f;
        float speed = 1.f;
        bool loop = false;

        enum class MoverBehavior {
            UpdateTransform,
            UpdateBones,
            None
        };

        MoverBehavior positionMoverBehavior = MoverBehavior::UpdateBones;
        MoverBehavior rotationMoverBehavior = MoverBehavior::UpdateBones;
        MoverBehavior scaleMoverBehavior = MoverBehavior::None;

        struct MovementUpToFrame {
            glm::vec3 position{ 0.f };
            glm::quat rotation{ 0.f, 0.f, 0.f, 0.f };
            glm::vec3 scale{ 1.f };
        };
        MovementUpToFrame movementUpToLastFrame;
    };
}