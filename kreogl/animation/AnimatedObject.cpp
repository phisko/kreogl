#include "AnimatedObject.hpp"

namespace kreogl {
    void AnimatedObject::tickAnimation(float deltaTime) noexcept {
        if (animation)
            animation->model->tick(deltaTime, *this);
    }
}