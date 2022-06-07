#include "AssImpModelLoader.hpp"

// stl
#include <cassert>
#include <filesystem>

// AssImp
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// stb
#include "stb_image.h"

// kreogl
#include "kreogl/texture/ImageTexture.hpp"
#include "AssImpAnimationLoader.hpp"
#include "AssImpSkeletonModel.hpp"
#include "AssImpToGLM.hpp"

namespace kreogl::AssImp {
    namespace {
        struct AssImpMeshData {
            std::vector<AnimatedModel::Vertex> vertices;
            std::vector<unsigned int> indices;
            AnimatedModel::MeshTextures textures;
        };

        struct AssImpModelData {
            std::vector<AssImpMeshData> meshes;
        };

        struct impl {
            static AssImpModelData loadModelData(const char * file, const aiScene & scene) noexcept {
                AssImpModelData ret;

                const auto dir = std::filesystem::path(file).parent_path().string();
                processNode(ret, dir, scene, *scene.mRootNode);

                return ret;
            }

            static void processNode(AssImpModelData & modelData, const std::string & directory, const aiScene & scene, const aiNode & node) noexcept {
                for (unsigned int i = 0; i < node.mNumMeshes; ++i) {
                    const auto mesh = scene.mMeshes[node.mMeshes[i]];
                    modelData.meshes.push_back(processMesh(*mesh, scene, directory));
                }

                for (unsigned int i = 0; i < node.mNumChildren; ++i)
                    processNode(modelData, directory, scene, *node.mChildren[i]);
            }

            static AssImpMeshData processMesh(const aiMesh & mesh, const aiScene & scene, const std::string & directory) noexcept {
                AssImpMeshData ret;

                for (unsigned int i = 0; i < mesh.mNumVertices; ++i) {
                    AnimatedModel::Vertex vertex;

                    vertex.position.x = mesh.mVertices[i].x;
                    vertex.position.y = mesh.mVertices[i].y;
                    vertex.position.z = mesh.mVertices[i].z;

                    vertex.normal.x = mesh.mNormals[i].x;
                    vertex.normal.y = mesh.mNormals[i].y;
                    vertex.normal.z = mesh.mNormals[i].z;

                    if (mesh.mTextureCoords[0] != nullptr) {
                        vertex.texCoords.x = mesh.mTextureCoords[0][i].x;
                        vertex.texCoords.y = mesh.mTextureCoords[0][i].y;
                    }
                    else
                        vertex.texCoords = { 0.f, 0.f };

                    ret.vertices.push_back(vertex);
                }

                // for each bone
                for (unsigned int boneIndex = 0; boneIndex < mesh.mNumBones; ++boneIndex) {
                    const auto & bone = *mesh.mBones[boneIndex];
                    // for each weight (vertex it has an influence on)
                    for (unsigned int weightIndex = 0; weightIndex < bone.mNumWeights; ++weightIndex) {
                        const auto & weight = bone.mWeights[weightIndex];
                        auto & vertex = ret.vertices[weight.mVertexId];

                        // add this bone to the vertex
                        bool found = false;
                        for (unsigned int i = 0; i < KREOGL_BONE_INFO_PER_VERTEX; ++i)
                            if (vertex.boneWeights[i] == 0.f) {
                                found = true;
                                vertex.boneWeights[i] = weight.mWeight;
                                vertex.boneIDs[i] = boneIndex;
                                break;
                            }
                        assert(found); // Need to increase KREOGL_BONE_INFO_PER_VERTEX
                        if (!found) {
                            float smallestWeight = std::numeric_limits<float>::max();
                            unsigned int smallestIndex = 0;
                            for (unsigned int k = 0; k < KREOGL_BONE_INFO_PER_VERTEX; ++k)
                                if (vertex.boneWeights[k] < smallestWeight) {
                                    smallestWeight = vertex.boneWeights[k];
                                    smallestIndex = k;
                                }
                            if (weight.mWeight > smallestWeight) {
                                vertex.boneWeights[smallestIndex] = weight.mWeight;
                                vertex.boneIDs[smallestIndex] = boneIndex;
                            }
                        }
                    }
                }

                // for models with no skeleton
                for (auto & vertex : ret.vertices)
                    if (vertex.boneWeights[0] == 0.f) {
                        vertex.boneWeights[0] = 1.f;
                        vertex.boneIDs[0] = 0;
                    }

                for (unsigned int i = 0; i < mesh.mNumFaces; ++i) {
                    const auto & face = mesh.mFaces[i];
                    for (unsigned int j = 0; j < face.mNumIndices; ++j)
                        ret.indices.push_back(face.mIndices[j]);
                }

                if (mesh.mMaterialIndex >= 0) {
                    const auto & material = *scene.mMaterials[mesh.mMaterialIndex];

                    ret.textures.diffuseTextures = loadTextures(directory, material, aiTextureType_DIFFUSE, scene);
                    ret.textures.specularTextures = loadTextures(directory, material, aiTextureType_SPECULAR, scene);

                    aiColor3D color{ 0.f, 0.f, 0.f };
                    material.Get(AI_MATKEY_COLOR_DIFFUSE, color);
                    ret.textures.diffuseColor = glm::vec4(color.r, color.g, color.b, 1.f);
                    material.Get(AI_MATKEY_COLOR_SPECULAR, color);
                    ret.textures.specularColor = glm::vec4(color.r, color.g, color.b, 1.f);
                }
                else
                    assert(false); // Unknown material

                return ret;
            }

            static std::vector<kreogl::ImageTexture> loadTextures(const std::string & directory, const aiMaterial & material, aiTextureType type, const aiScene & scene) noexcept {
                std::vector<kreogl::ImageTexture> ret;

                for (unsigned int i = 0; i < material.GetTextureCount(type); ++i) {
                    aiString path;
                    material.GetTexture(type, i, &path);
                    const auto cPath = path.C_Str();
                    if (const auto texture = scene.GetEmbeddedTexture(cPath))
                        ret.emplace_back(texture->pcData, texture->mWidth, texture->mHeight);
                    else
                        ret.emplace_back((directory + '/' + cPath).c_str());
                }

                return ret;
            }

            static AssImpSkeletonModel loadSkeleton(const aiScene & scene) noexcept {
                std::vector<const aiNode *> allNodes;
                addNode(*scene.mRootNode, allNodes);

                AssImpSkeletonModel ret;
                ret.rootNode = scene.mRootNode;
                for (unsigned int meshIndex = 0; meshIndex < scene.mNumMeshes; ++meshIndex) {
                    const auto & mesh = *scene.mMeshes[meshIndex];

                    SkeletonModel::Mesh skeletonMesh;
                    AssImpSkeletonModel::Mesh assimpSkeletonMesh;
                    for (unsigned int boneIndex = 0; boneIndex < mesh.mNumBones; ++boneIndex) {
                        const auto & bone = *mesh.mBones[boneIndex];
                        const auto & name = bone.mName;

                        assimpSkeletonMesh.bones.push_back(AssImpSkeletonModel::Mesh::Bone{
                            .node = findNode(allNodes, name),
                            .offset = toglm(bone.mOffsetMatrix)
                        });
                        skeletonMesh.boneNames.push_back(name.C_Str());
                    }
                    ret.assimpMeshes.emplace_back(std::move(assimpSkeletonMesh));
                    ret.meshes.emplace_back(std::move(skeletonMesh));
                }

                ret.globalInverseTransform = glm::inverse(toglm(scene.mRootNode->mTransformation));
                return ret;
            }

            static void addNode(const aiNode & node, std::vector<const aiNode *> & allNodes) noexcept {
                allNodes.push_back(&node);
                for (unsigned int i = 0; i < node.mNumChildren; ++i)
                    addNode(*node.mChildren[i], allNodes);
            }

            static const aiNode * findNode(const std::vector<const aiNode *> & allNodes, const aiString & name) noexcept {
                const auto node = std::ranges::find_if(allNodes, [&name](const aiNode * node) noexcept {
                    return node->mName == name;
                });

                if (node == allNodes.end()) {
                    assert(false);
                    return nullptr;
                }

                return *node;
            }
        };
    }

    std::optional<AssImpAnimatedModel> loadModel(const char * file) noexcept {
        AssImpAnimatedModel ret{ VertexSpecification::positionNormalTexcoordsBoneweightsBoneids };

        ret.importer = std::make_unique<Assimp::Importer>();
        const auto scene = ret.importer->ReadFile(file, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals /*| aiProcess_OptimizeMeshes*/ | aiProcess_JoinIdenticalVertices);
        const auto succeeded = scene != nullptr && !(scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) && scene->mRootNode != nullptr;
        if (!succeeded) {
            assert(false);
            return std::nullopt;
        }

        auto assimpModelData = impl::loadModelData(file, *scene);

        ModelData modelData {
            .vertexAttributeOffsets = {
                offsetof(AnimatedModel::Vertex, position),
                offsetof(AnimatedModel::Vertex, normal),
                offsetof(AnimatedModel::Vertex, texCoords),
                offsetof(AnimatedModel::Vertex, boneWeights),
                offsetof(AnimatedModel::Vertex, boneIDs)
            },
            .vertexSize = sizeof(AnimatedModel::Vertex)
        };

        for (const auto & assimpMeshData : assimpModelData.meshes)
            modelData.meshes.push_back(MeshData{
                .vertices = {
                    .nbElements = assimpMeshData.vertices.size(),
                    .elementSize = sizeof(AnimatedModel::Vertex),
                    .data = assimpMeshData.vertices.data()
                },
                .indices = {
                    .nbElements = assimpMeshData.indices.size(),
                    .elementSize = sizeof(unsigned int),
                    .data = assimpMeshData.indices.data()
                },
                .indexType = GL_UNSIGNED_INT
            });

        ret.init(modelData);

        for (auto & assimpMeshData : assimpModelData.meshes)
            ret.meshTextures.emplace_back(std::move(assimpMeshData.textures));

        ret.animations = kreogl::AssImp::loadAnimations(file, *scene);
        ret.skeleton = std::make_unique<AssImpSkeletonModel>(impl::loadSkeleton(*scene));
        return ret;
    }
}