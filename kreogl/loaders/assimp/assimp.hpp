#pragma once

// kreogl
#include "kreogl/animation/animated_model.hpp"
#include "kreogl/animation/animation_file.hpp"
#include "impl/assimp_model_data.hpp"

namespace kreogl::assimp {
	KREOGL_EXPORT bool is_supported_file_format(const char * file) noexcept;

	KREOGL_EXPORT std::unique_ptr<animated_model> load_animated_model(const char * file) noexcept;
	KREOGL_EXPORT std::unique_ptr<animation_file> load_animation_file(const char * file) noexcept;

	KREOGL_EXPORT assimp_model_data load_model_data(const char * file) noexcept;
	KREOGL_EXPORT std::unique_ptr<animated_model> load_animated_model(assimp_model_data && model_data) noexcept;
}