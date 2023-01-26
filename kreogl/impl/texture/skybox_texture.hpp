#pragma once

// stl
#include <optional>

// kreogl
#include "kreogl/impl/raii/texture.hpp"
#include "kreogl/impl/texture/texture_data.hpp"

namespace kreogl {
	struct KREOGL_EXPORT skybox_texture {
		skybox_texture(const texture_data & left, const texture_data & right, const texture_data & top, const texture_data & bottom, const texture_data & front, const texture_data & back) noexcept;

		skybox_texture() noexcept = default;
		skybox_texture(skybox_texture &&) noexcept = default;
		skybox_texture & operator=(skybox_texture &&) noexcept = default;

		std::optional<kreogl::texture> texture;
	};
}