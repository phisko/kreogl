#include "point_light.hpp"

// kreogl
#include "kreogl/impl/kreogl_profiling.hpp"

namespace kreogl {
	float point_light::get_radius() const noexcept {
		KREOGL_PROFILING_SCOPE;

		const auto max_channel = std::fmax(std::fmax(color.r, color.g), color.b);
		return (-attenuation_linear + sqrtf(attenuation_linear * attenuation_linear - 4.f * attenuation_quadratic * (attenuation_constant - 256.f * max_channel * diffuse_strength)))
			/ (2.f * attenuation_quadratic);
	}
}