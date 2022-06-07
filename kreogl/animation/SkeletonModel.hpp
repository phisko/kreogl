#pragma once

#include <string>
#include <vector>

namespace kreogl {
    struct SkeletonModel {
        virtual ~SkeletonModel() noexcept = default;

        struct Mesh {
            std::vector<std::string> boneNames;
        };

        std::vector<Mesh> meshes;
    };
}