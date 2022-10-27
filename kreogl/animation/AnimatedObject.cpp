#include "AnimatedObject.hpp"
#include "kreogl/impl/kreogl_profiling.hpp"

namespace kreogl {
	void AnimatedObject::tickAnimation(float deltaTime) noexcept {
		KREOGL_PROFILING_SCOPE;

		if (animation)
			animation->model->tick(deltaTime, *this);
	}
}