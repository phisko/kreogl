#pragma once

// stl
#include <vector>
#include <memory>

// assimp
#include <assimp/Importer.hpp>

// kreogl
#include "AssImpAnimationModel.hpp"

namespace kreogl::AssImp {
    struct AssImpAnimationFile {
        std::unique_ptr<Assimp::Importer> importer = nullptr;
        std::vector<AssImpAnimationModel> animations;
    };

    AssImpAnimationFile loadAnimations(const char * file) noexcept;
    std::vector<AssImpAnimationModel> loadAnimations(const char * sourceFile, const aiScene & scene) noexcept;
}