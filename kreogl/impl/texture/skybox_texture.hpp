#pragma once

// stl
#include <optional>

// kreogl
#include "kreogl/impl/raii/texture.hpp"

namespace kreogl {
	struct KREOGL_EXPORT skybox_texture {
		skybox_texture(const char * left, const char * right, const char * top, const char * bottom, const char * front, const char * back) noexcept;

		skybox_texture() noexcept = default;
		skybox_texture(skybox_texture &&) noexcept = default;
		skybox_texture & operator=(skybox_texture &&) noexcept = default;

		std::optional<kreogl::texture> texture;
	};
}