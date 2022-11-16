#pragma once

// kreogl
#include "kreogl/animation/AnimatedModel.hpp"
#include "kreogl/animation/AnimationFile.hpp"

namespace kreogl::AssImp {
	KREOGL_EXPORT bool isSupportedFileFormat(const char * file) noexcept;
	KREOGL_EXPORT std::unique_ptr<AnimatedModel> loadAnimatedModel(const char * file) noexcept;
	KREOGL_EXPORT std::unique_ptr<AnimationFile> loadAnimationFile(const char * file) noexcept;
}