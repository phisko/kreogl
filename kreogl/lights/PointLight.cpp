#include "PointLight.hpp"

namespace kreogl {
    float PointLight::getRadius() const noexcept {
        const auto maxChannel = std::fmax(std::fmax(color.r, color.g), color.b);
        return (-attenuationLinear + sqrtf(attenuationLinear * attenuationLinear - 4.f * attenuationQuadratic * (attenuationConstant - 256.f * maxChannel * diffuseStrength)))
               /
               (2.f * attenuationQuadratic);
    }
}