#include "animated_object.hpp"

// kreogl
#include "kreogl/impl/kreogl_profiling.hpp"

namespace kreogl {
	void animated_object::tick_animation(float delta_time) noexcept {
		KREOGL_PROFILING_SCOPE;

		if (animation)
			animation->model->tick(delta_time, *this);
	}
}