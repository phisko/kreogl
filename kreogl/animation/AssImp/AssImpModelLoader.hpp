#pragma once

#include "AssImpAnimatedModel.hpp"
#include <optional>

namespace kreogl::AssImp {
    std::optional<AssImpAnimatedModel> loadModel(const char * file) noexcept;
}