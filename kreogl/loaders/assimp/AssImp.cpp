#include "AssImp.hpp"

// stl
#include <filesystem>
#include <span>

// assimp
#include <assimp/postprocess.h>

// kreogl
#include "kreogl/impl/kreogl_profiling.hpp"
#include "impl/AssImpAnimatedModel.hpp"
#include "impl/AssImpAnimationFile.hpp"
#include "impl/AssImpAnimationModel.hpp"
#include "impl/AssImpSkeletonModel.hpp"
#include "impl/AssImpToGLM.hpp"

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
				KREOGL_PROFILING_SCOPE;

				AssImpModelData ret;

				const auto dir = std::filesystem::path(file).parent_path().string();
				processNode(ret, dir, scene, *scene.mRootNode);

				return ret;
			}

			static void processNode(AssImpModelData & modelData, const std::string & directory, const aiScene & scene, const aiNode & node) noexcept {
				KREOGL_PROFILING_SCOPE;

				for (const auto meshIndex : std::span(node.mMeshes, node.mNumMeshes)) {
					const auto mesh = scene.mMeshes[meshIndex];
					modelData.meshes.push_back(processMesh(*mesh, scene, directory));
				}

				for (const auto child : std::span(node.mChildren, node.mNumChildren))
					processNode(modelData, directory, scene, *child);
			}

			static AssImpMeshData processMesh(const aiMesh & mesh, const aiScene & scene, const std::string & directory) noexcept {
				KREOGL_PROFILING_SCOPE;

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
					} else
						vertex.texCoords = { 0.f, 0.f };

					ret.vertices.push_back(vertex);
				}

				// for each bone
				for (unsigned int boneIndex = 0; boneIndex < mesh.mNumBones; ++boneIndex) {
					const auto & bone = *mesh.mBones[boneIndex];
					// for each weight (vertex it has an influence on)
					for (const auto & weight : std::span(bone.mWeights, bone.mNumWeights)) {
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

				for (const auto & face : std::span(mesh.mFaces, mesh.mNumFaces))
					for (const auto index : std::span(face.mIndices, face.mNumIndices))
						ret.indices.push_back(index);

				if (mesh.mMaterialIndex >= 0) {
					const auto & material = *scene.mMaterials[mesh.mMaterialIndex];

					ret.textures.diffuseTextures = loadTextures(directory, material, aiTextureType_DIFFUSE, scene);
					ret.textures.specularTextures = loadTextures(directory, material, aiTextureType_SPECULAR, scene);

					aiColor3D color{ 0.f, 0.f, 0.f };
					material.Get(AI_MATKEY_COLOR_DIFFUSE, color);
					ret.textures.diffuseColor = glm::vec4(color.r, color.g, color.b, 1.f);
					material.Get(AI_MATKEY_COLOR_SPECULAR, color);
					ret.textures.specularColor = glm::vec4(color.r, color.g, color.b, 1.f);
				} else
					assert(false); // Unknown material

				return ret;
			}

			static std::vector<kreogl::ImageTexture> loadTextures(const std::string & directory, const aiMaterial & material, aiTextureType type, const aiScene & scene) noexcept {
				KREOGL_PROFILING_SCOPE;

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
				KREOGL_PROFILING_SCOPE;

				std::vector<const aiNode *> allNodes;
				addNode(*scene.mRootNode, allNodes);

				AssImpSkeletonModel ret;
				ret.rootNode = scene.mRootNode;
				for (const auto mesh : std::span(scene.mMeshes, scene.mNumMeshes)) {
					SkeletonModel::Mesh skeletonMesh;
					AssImpSkeletonModel::Mesh assimpSkeletonMesh;
					for (const auto bone : std::span(mesh->mBones, mesh->mNumBones)) {
						const auto & name = bone->mName;
						assimpSkeletonMesh.bones.push_back(AssImpSkeletonModel::Mesh::Bone{
							.node = findNode(allNodes, name),
							.offset = toglm(bone->mOffsetMatrix)
						});
						skeletonMesh.boneNames.emplace_back(name.C_Str());
					}
					ret.assimpMeshes.emplace_back(std::move(assimpSkeletonMesh));
					ret.meshes.emplace_back(std::move(skeletonMesh));
				}

				return ret;
			}

			static void addNode(const aiNode & node, std::vector<const aiNode *> & allNodes) noexcept {
				KREOGL_PROFILING_SCOPE;

				allNodes.push_back(&node);
				for (const auto child : std::span(node.mChildren, node.mNumChildren))
					addNode(*child, allNodes);
			}

			static const aiNode * findNode(const std::vector<const aiNode *> & allNodes, const aiString & name) noexcept {
				KREOGL_PROFILING_SCOPE;

				const auto node = std::ranges::find_if(allNodes, [&name](const aiNode * node) noexcept {
					return node->mName == name;
				});

				if (node == allNodes.end()) {
					assert(false);
					return nullptr;
				}

				return *node;
			}

			static std::vector<std::unique_ptr<AnimationModel>> loadAnimations(const char * sourceFile, const aiScene & scene) noexcept {
				KREOGL_PROFILING_SCOPE;

				std::vector<std::unique_ptr<AnimationModel>> ret;

				for (const auto assimpAnim : std::span(scene.mAnimations, scene.mNumAnimations)) {
					auto anim = std::make_unique<AssImpAnimationModel>();
					anim->assimpAnimation = assimpAnim;
					anim->name = sourceFile;
					anim->name += '/';
					anim->name += assimpAnim->mName.C_Str();
					anim->ticksPerSecond = (float)(assimpAnim->mTicksPerSecond != 0 ? assimpAnim->mTicksPerSecond : 25.0);
					anim->totalTime = (float)assimpAnim->mDuration / anim->ticksPerSecond;
					ret.emplace_back(std::move(anim));
				}

				return ret;
			}
		};
	}

	bool isSupportedFileFormat(const char * file) noexcept {
		Assimp::Importer importer;
		const auto extension = std::filesystem::path(file).extension().string();
		return importer.IsExtensionSupported(extension);
	}

	std::unique_ptr<AnimatedModel> loadAnimatedModel(const char * file) noexcept {
		KREOGL_PROFILING_SCOPE;

		auto importer = std::make_unique<Assimp::Importer>();

		const auto scene = importer->ReadFile(file, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals /*| aiProcess_OptimizeMeshes*/ | aiProcess_JoinIdenticalVertices);
		const auto succeeded = scene != nullptr && !(scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) && scene->mRootNode != nullptr;
		if (!succeeded)
			return nullptr;

		auto assimpModelData = impl::loadModelData(file, *scene);

		ModelData modelData{
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

		auto ret = std::make_unique<AssImpAnimatedModel>(VertexSpecification::skeletal, modelData);
		ret->importer = std::move(importer);

		for (auto & assimpMeshData : assimpModelData.meshes)
			ret->meshTextures.emplace_back(std::move(assimpMeshData.textures));

		ret->animations = impl::loadAnimations(file, *scene);
		ret->skeleton = std::make_unique<AssImpSkeletonModel>(impl::loadSkeleton(*scene));
		return ret;
	}

	std::unique_ptr<AnimationFile> loadAnimationFile(const char * file) noexcept {
		KREOGL_PROFILING_SCOPE;

		auto ret = std::make_unique<AssImpAnimationFile>();

		ret->importer = std::make_unique<Assimp::Importer>();
		const auto scene = ret->importer->ReadFile(file, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals /*| aiProcess_OptimizeMeshes*/ | aiProcess_JoinIdenticalVertices);
		if (scene == nullptr || scene->mRootNode == nullptr) {
			assert(false);
			return ret;
		}

		ret->animations = impl::loadAnimations(file, *scene);
		return ret;
	}
}