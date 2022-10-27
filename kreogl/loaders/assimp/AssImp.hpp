#pragma once

// kreogl
#include "kreogl/animation/AnimatedModel.hpp"
#include "kreogl/animation/AnimationFile.hpp"

namespace kreogl::AssImp {
	bool isSupportedFileFormat(const char * file) noexcept;
	std::unique_ptr<AnimatedModel> loadAnimatedModel(const char * file) noexcept;
	std::unique_ptr<AnimationFile> loadAnimationFile(const char * file) noexcept;
}