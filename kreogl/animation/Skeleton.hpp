#pragma once

// stl
#include <vector>

// glm
#include <glm/glm.hpp>

// kreogl
#include "SkeletonModel.hpp"

namespace kreogl {
    struct Skeleton {
        struct Mesh {
            // Indexed by SkeletonModel::Mesh::boneNames
            std::vector<glm::mat4> boneMatsBoneSpace;
            std::vector<glm::mat4> boneMatsMeshSpace;
        };

        const SkeletonModel * model = nullptr;
        std::vector<Mesh> meshes;
    };
}