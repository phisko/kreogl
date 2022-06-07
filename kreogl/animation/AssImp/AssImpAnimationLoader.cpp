#include "AssImpAnimationLoader.hpp"

// stl
#include <memory>

// assimp
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

namespace kreogl::AssImp {
    AssImpAnimationFile loadAnimations(const char * file) noexcept {
        AssImpAnimationFile ret;

        ret.importer = std::make_unique<Assimp::Importer>();
        const auto scene = ret.importer->ReadFile(file, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals /*| aiProcess_OptimizeMeshes*/ | aiProcess_JoinIdenticalVertices);
        if (scene == nullptr || scene->mRootNode == nullptr) {
            assert(false);
            return ret;
        }

        ret.animations = loadAnimations(file, *scene);
        return ret;
    }

    std::vector<AssImpAnimationModel> loadAnimations(const char * sourceFile, const aiScene & scene) noexcept {
        std::vector<AssImpAnimationModel> ret;

        for (unsigned int i = 0; i < scene.mNumAnimations; ++i) {
            const auto & assimpAnim = *scene.mAnimations[i];
            AssImpAnimationModel anim;
            anim.assimpAnimation = &assimpAnim;
            anim.name = sourceFile;
            anim.name += '/';
            anim.name += assimpAnim.mName.C_Str();
            anim.ticksPerSecond = (float)(assimpAnim.mTicksPerSecond != 0 ? assimpAnim.mTicksPerSecond : 25.0);
            anim.totalTime = (float)assimpAnim.mDuration / anim.ticksPerSecond;
            ret.emplace_back(std::move(anim));
        }

        return ret;
    }
}