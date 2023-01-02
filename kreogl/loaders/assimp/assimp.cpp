#include "assimp.hpp"

// stl
#include <filesystem>
#include <span>

// assimp
#include <assimp/postprocess.h>

// kreogl
#include "kreogl/impl/kreogl_profiling.hpp"
#include "impl/assimp_animated_model.hpp"
#include "impl/assimp_animation_file.hpp"
#include "impl/assimp_animation_model.hpp"
#include "impl/assimp_skeleton_model.hpp"
#include "impl/assimp_to_glm.hpp"

namespace kreogl::assimp {
	namespace {
		struct assimp_mesh_data {
			std::vector<animated_model::vertex> vertices;
			std::vector<unsigned int> indices;
			animated_model::mesh_textures textures;
		};

		struct assimp_model_data {
			std::vector<assimp_mesh_data> meshes;
		};

		struct impl {
			static assimp_model_data load_model_data(const char * file, const aiScene & scene) noexcept {
				KREOGL_PROFILING_SCOPE;

				assimp_model_data ret;

				const auto dir = std::filesystem::path(file).parent_path().string();
				process_node(ret, dir, scene, *scene.mRootNode);

				return ret;
			}

			static void process_node(assimp_model_data & model_data, const std::string & directory, const aiScene & scene, const aiNode & node) noexcept {
				KREOGL_PROFILING_SCOPE;

				for (const auto mesh_index : std::span(node.mMeshes, node.mNumMeshes)) {
					const auto mesh = scene.mMeshes[mesh_index];
					model_data.meshes.push_back(process_mesh(*mesh, scene, directory));
				}

				for (const auto child : std::span(node.mChildren, node.mNumChildren))
					process_node(model_data, directory, scene, *child);
			}

			static assimp_mesh_data process_mesh(const aiMesh & mesh, const aiScene & scene, const std::string & directory) noexcept {
				KREOGL_PROFILING_SCOPE;

				assimp_mesh_data ret;

				for (unsigned int i = 0; i < mesh.mNumVertices; ++i) {
					animated_model::vertex vertex;

					vertex.position.x = mesh.mVertices[i].x;
					vertex.position.y = mesh.mVertices[i].y;
					vertex.position.z = mesh.mVertices[i].z;

					vertex.normal.x = mesh.mNormals[i].x;
					vertex.normal.y = mesh.mNormals[i].y;
					vertex.normal.z = mesh.mNormals[i].z;

					if (mesh.mTextureCoords[0] != nullptr) {
						vertex.tex_coords.x = mesh.mTextureCoords[0][i].x;
						vertex.tex_coords.y = mesh.mTextureCoords[0][i].y;
					}
					else
						vertex.tex_coords = { 0.f, 0.f };

					ret.vertices.push_back(vertex);
				}

				// for each bone
				for (unsigned int bone_index = 0; bone_index < mesh.mNumBones; ++bone_index) {
					const auto & bone = *mesh.mBones[bone_index];
					// for each weight (vertex it has an influence on)
					for (const auto & weight : std::span(bone.mWeights, bone.mNumWeights)) {
						auto & vertex = ret.vertices[weight.mVertexId];

						// add this bone to the vertex
						bool found = false;
						for (unsigned int i = 0; i < KREOGL_BONE_INFO_PER_VERTEX; ++i)
							if (vertex.bone_weights[i] == 0.f) {
								found = true;
								vertex.bone_weights[i] = weight.mWeight;
								vertex.bone_ids[i] = bone_index;
								break;
							}
						assert(found); // Need to increase KREOGL_BONE_INFO_PER_VERTEX
						if (!found) {
							float smallest_weight = std::numeric_limits<float>::max();
							unsigned int smallest_index = 0;
							for (unsigned int k = 0; k < KREOGL_BONE_INFO_PER_VERTEX; ++k)
								if (vertex.bone_weights[k] < smallest_weight) {
									smallest_weight = vertex.bone_weights[k];
									smallest_index = k;
								}
							if (weight.mWeight > smallest_weight) {
								vertex.bone_weights[smallest_index] = weight.mWeight;
								vertex.bone_ids[smallest_index] = bone_index;
							}
						}
					}
				}

				// for models with no skeleton
				for (auto & vertex : ret.vertices)
					if (vertex.bone_weights[0] == 0.f) {
						vertex.bone_weights[0] = 1.f;
						vertex.bone_ids[0] = 0;
					}

				for (const auto & face : std::span(mesh.mFaces, mesh.mNumFaces))
					for (const auto index : std::span(face.mIndices, face.mNumIndices))
						ret.indices.push_back(index);

				if (mesh.mMaterialIndex >= 0) {
					const auto & material = *scene.mMaterials[mesh.mMaterialIndex];

					ret.textures.diffuse_textures = load_textures(directory, material, aiTextureType_DIFFUSE, scene);
					ret.textures.specular_textures = load_textures(directory, material, aiTextureType_SPECULAR, scene);

					aiColor3D color{ 0.f, 0.f, 0.f };
					material.Get(AI_MATKEY_COLOR_DIFFUSE, color);
					ret.textures.diffuse_color = glm::vec4(color.r, color.g, color.b, 1.f);
					material.Get(AI_MATKEY_COLOR_SPECULAR, color);
					ret.textures.specular_color = glm::vec4(color.r, color.g, color.b, 1.f);
				}
				else
					assert(false); // Unknown material

				return ret;
			}

			static std::vector<kreogl::image_texture> load_textures(const std::string & directory, const aiMaterial & material, aiTextureType type, const aiScene & scene) noexcept {
				KREOGL_PROFILING_SCOPE;

				std::vector<kreogl::image_texture> ret;

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

			static assimp_skeleton_model load_skeleton(const aiScene & scene) noexcept {
				KREOGL_PROFILING_SCOPE;

				std::vector<const aiNode *> all_nodes;
				add_node(*scene.mRootNode, all_nodes);

				assimp_skeleton_model ret;
				ret.root_node = scene.mRootNode;
				for (const auto mesh : std::span(scene.mMeshes, scene.mNumMeshes)) {
					skeleton_model::mesh skeleton_mesh;
					assimp_skeleton_model::mesh assimp_skeleton_mesh;
					for (const auto bone : std::span(mesh->mBones, mesh->mNumBones)) {
						const auto & name = bone->mName;
						assimp_skeleton_mesh.bones.push_back(assimp_skeleton_model::mesh::bone{
							.node = find_node(all_nodes, name),
							.offset = toglm(bone->mOffsetMatrix),
						});
						skeleton_mesh.bone_names.emplace_back(name.C_Str());
					}
					ret.assimp_meshes.emplace_back(std::move(assimp_skeleton_mesh));
					ret.meshes.emplace_back(std::move(skeleton_mesh));
				}

				return ret;
			}

			static void add_node(const aiNode & node, std::vector<const aiNode *> & all_nodes) noexcept {
				KREOGL_PROFILING_SCOPE;

				all_nodes.push_back(&node);
				for (const auto child : std::span(node.mChildren, node.mNumChildren))
					add_node(*child, all_nodes);
			}

			static const aiNode * find_node(const std::vector<const aiNode *> & all_nodes, const aiString & name) noexcept {
				KREOGL_PROFILING_SCOPE;

				const auto node = std::ranges::find_if(all_nodes, [&name](const aiNode * node) noexcept {
					return node->mName == name;
				});

				if (node == all_nodes.end()) {
					assert(false);
					return nullptr;
				}

				return *node;
			}

			static std::vector<std::unique_ptr<animation_model>> load_animations(const char * source_file, const aiScene & scene) noexcept {
				KREOGL_PROFILING_SCOPE;

				std::vector<std::unique_ptr<animation_model>> ret;

				for (const auto assimp_anim : std::span(scene.mAnimations, scene.mNumAnimations)) {
					auto anim = std::make_unique<assimp_animation_model>();
					anim->assimp_animation = assimp_anim;
					anim->name = source_file;
					anim->name += '/';
					anim->name += assimp_anim->mName.C_Str();
					anim->ticks_per_second = (float)(assimp_anim->mTicksPerSecond != 0 ? assimp_anim->mTicksPerSecond : 25.0);
					anim->total_time = (float)assimp_anim->mDuration / anim->ticks_per_second;
					ret.emplace_back(std::move(anim));
				}

				return ret;
			}
		};
	}

	bool is_supported_file_format(const char * file) noexcept {
		Assimp::Importer importer;
		const auto extension = std::filesystem::path(file).extension().string();
		return importer.IsExtensionSupported(extension);
	}

	std::unique_ptr<animated_model> load_animated_model(const char * file) noexcept {
		KREOGL_PROFILING_SCOPE;

		auto importer = std::make_unique<Assimp::Importer>();

		const auto scene = importer->ReadFile(file, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals /*| aiProcess_OptimizeMeshes*/ | aiProcess_JoinIdenticalVertices);
		const auto succeeded = scene != nullptr && !(scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) && scene->mRootNode != nullptr;
		if (!succeeded)
			return nullptr;

		auto assimp_model_data = impl::load_model_data(file, *scene);

		model_data model_data{
			.vertex_attribute_offsets = {
				offsetof(animated_model::vertex, position),
				offsetof(animated_model::vertex, normal),
				offsetof(animated_model::vertex, tex_coords),
				offsetof(animated_model::vertex, bone_weights),
				offsetof(animated_model::vertex, bone_ids),
			},
			.vertex_size = sizeof(animated_model::vertex)
		};

		for (const auto & assimp_mesh_data : assimp_model_data.meshes)
			model_data.meshes.push_back(mesh_data{
				.vertices = {
					.nb_elements = assimp_mesh_data.vertices.size(),
					.element_size = sizeof(animated_model::vertex),
					.data = assimp_mesh_data.vertices.data(),
				},
				.indices = {
					.nb_elements = assimp_mesh_data.indices.size(),
					.element_size = sizeof(unsigned int),
					.data = assimp_mesh_data.indices.data(),
				},
				.index_type = GL_UNSIGNED_INT,
			});

		auto ret = std::make_unique<assimp_animated_model>(vertex_specification::skeletal, model_data);
		ret->importer = std::move(importer);

		for (auto & assimp_mesh_data : assimp_model_data.meshes)
			ret->textures.emplace_back(std::move(assimp_mesh_data.textures));

		ret->animations = impl::load_animations(file, *scene);
		ret->skeleton = std::make_unique<assimp_skeleton_model>(impl::load_skeleton(*scene));
		return ret;
	}

	std::unique_ptr<animation_file> load_animation_file(const char * file) noexcept {
		KREOGL_PROFILING_SCOPE;

		auto ret = std::make_unique<assimp_animation_file>();

		ret->importer = std::make_unique<Assimp::Importer>();
		const auto scene = ret->importer->ReadFile(file, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals /*| aiProcess_OptimizeMeshes*/ | aiProcess_JoinIdenticalVertices);
		if (scene == nullptr || scene->mRootNode == nullptr) {
			assert(false);
			return ret;
		}

		ret->animations = impl::load_animations(file, *scene);
		return ret;
	}
}