#pragma once

// glm
#include <glm/glm.hpp>

// kreogl
#include "impl/texture/ImageTexture.hpp"

namespace kreogl {
	struct KREOGL_EXPORT Sprite {
		const ImageTexture * texture = nullptr;
		glm::mat4 transform{ 1.f };
		glm::vec4 color{ 1.f };
		glm::vec4 userData{ 0.f };
	};

	struct KREOGL_EXPORT Sprite2D : Sprite {};
	struct KREOGL_EXPORT Sprite3D : Sprite {};
}