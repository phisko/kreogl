#pragma once

#include <optional>
#include "kreogl/impl/RAII/Texture.hpp"

namespace kreogl {
	struct SkyboxTexture {
		SkyboxTexture(const char * left, const char * right, const char * top, const char * bottom, const char * front, const char * back) noexcept;

		SkyboxTexture() noexcept = default;
		SkyboxTexture(SkyboxTexture &&) noexcept = default;
		SkyboxTexture & operator=(SkyboxTexture &&) noexcept = default;

		std::optional<Texture> texture;
	};
}