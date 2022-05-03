#include "PointLight.hpp"

namespace kreogl {
    float PointLight::getRadius() const noexcept {
        const auto maxChannel = std::fmax(std::fmax(color.r, color.g), color.b);
        return (-linear + sqrtf(linear * linear - 4.f * quadratic * (constant - 256.f * maxChannel * diffuseStrength)))
               /
               (2.f * quadratic);
    }
}