// stl
#include <filesystem>

// kreogl
#include "kreogl/window.hpp"
#include "kreogl/world.hpp"
#include "kreogl/animation/animated_object.hpp"
#include "kreogl/loaders/assimp/assimp.hpp"
#include "kreogl/loaders/polyvox/polyvox.hpp"
#include "kreogl/impl/kreogl_profiling.hpp"

// polyvox
#include "PolyVox/RawVolume.h"

// GLFW
#include <GLFW/glfw3.h>
#include <glm/gtx/rotate_vector.hpp>

// Functions to automatically move the camera to a set of fixed positions
// Automatically stops when the user moves the camera, and starts again by pressing 'R' (handled by input::process_input)
namespace camera_rotation {
	// Determines whether to apply the rotation or not
	static bool s_rotating_fixed_cameras = true;

	static void rotate_fixed_cameras(kreogl::window & window) noexcept {
		if (!s_rotating_fixed_cameras)
			return;

		struct camera_params {
			glm::vec3 position;
			glm::vec3 direction;
		};

		static const camera_params params[] = {
			{
				// Inside the point light demo
				.position = { -9.f, 7.f, -7.5f },
				.direction = { 1.3f, -1.f, 0.6f },
			},
			{
				// Global view
				.position = { 24.f, -7.f, -23.f },
				.direction = { -1.2f, -0.8f, 0.f },
			},
			{
				// Close up of blocks
				.position = { 0.f, -40.f, -20.f },
				.direction = { -1.f, -0.5f, 0.6f },
			}
		};
		static size_t current = 0;

		static const auto time_between_changes = std::chrono::seconds(3);
		static auto last_change_time = std::chrono::system_clock::now() - time_between_changes * 2; // make sure we trigger a change on first frame

		const auto now = std::chrono::system_clock::now();
		if (now - last_change_time > time_between_changes) {
			const auto & current_params = params[current];
			auto & camera = window.get_default_camera();
			camera.set_position(current_params.position);
			camera.set_direction(current_params.direction);

			current = (current + 1) % std::extent_v<decltype(params)>;
			last_change_time = now;
		}
	}
}

// Functions to move the camera around
namespace input {
	static kreogl::window * s_window = nullptr;
	static std::unordered_map<int, bool> s_keys_pressed;

	static constexpr auto MOUSE_SENSITIVITY = .01f;
	static constexpr auto MOVEMENT_SPEED = 5.f;

	struct camera_vectors {
		glm::vec3 front;
		glm::vec3 right;
		glm::vec3 up;
	};

	static camera_vectors get_camera_vectors(const kreogl::camera & camera) noexcept {
		const auto front = camera.get_direction();
		const auto right = glm::cross(front, { 0.f, 1.f, 0.f });
		const auto up = glm::cross(right, front);
		return { front, right, up };
	}

	static void setup_input(kreogl::window & window) noexcept {
		s_window = &window;

		glfwSetInputMode(window.get_glfw_window(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		glfwSetCursorPosCallback(window.get_glfw_window(), [](GLFWwindow * window, double x_pos, double y_pos) noexcept {
			static glm::vec2 last_pos = { (float)x_pos, (float)y_pos };

			const glm::vec2 new_pos = { (float)x_pos, (float)y_pos };
			const glm::vec2 delta = new_pos - last_pos;
			last_pos = new_pos;

			auto & camera = s_window->get_default_camera();

			auto direction = camera.get_direction();
			const auto vectors = get_camera_vectors(camera);
			direction = glm::rotate(direction, -delta.x * MOUSE_SENSITIVITY, vectors.up);
			direction = glm::rotate(direction, -delta.y * MOUSE_SENSITIVITY, vectors.right);
			camera.set_direction(direction);
		});

		glfwSetKeyCallback(window.get_glfw_window(), [](GLFWwindow * window, int key, int scancode, int action, int mods) noexcept {
			if (action == GLFW_PRESS) {
				camera_rotation::s_rotating_fixed_cameras = false; // Disable automatic camera changes if user wants to move around
				s_keys_pressed[key] = true;
			}
			else if (action == GLFW_RELEASE)
				s_keys_pressed[key] = false;
		});
	}

	static void process_input(kreogl::window & window, float delta_time) noexcept {
		auto & camera = window.get_default_camera();

		const auto vectors = get_camera_vectors(camera);
		const auto velocity = MOVEMENT_SPEED * delta_time;

		auto position = camera.get_position();

		if (s_keys_pressed['W'])
			position += vectors.front * velocity;
		if (s_keys_pressed['S'])
			position -= vectors.front * velocity;
		if (s_keys_pressed['A'])
			position -= vectors.right * velocity;
		if (s_keys_pressed['D'])
			position += vectors.right * velocity;

		if (s_keys_pressed[GLFW_KEY_LEFT_SHIFT])
			position += vectors.up * velocity;
		if (s_keys_pressed[GLFW_KEY_LEFT_CONTROL])
			position -= vectors.up * velocity;

		if (s_keys_pressed['R'])
			camera_rotation::s_rotating_fixed_cameras = true;

		camera.set_position(position);
	}
}

// Functions that create and return PolyVox models
namespace voxel_models {
	// Vertex type for PolyVox volumes
	struct vertex_data {
		glm::vec3 color;

		bool operator==(const vertex_data & rhs) const {
			return color == rhs.color;
		}

		// Used to know if vertex is empty
		bool operator==(size_t i) const {
			return color == glm::vec3{ 0.f };
		}

		// Used to know if vertex is empty
		bool operator>(size_t i) const {
			return color != glm::vec3{ 0.f };
		}
	};

	// Block of 3 RGB voxels
	static const kreogl::model & get_block_model() noexcept {
		static const auto model = [] {
			PolyVox::RawVolume<vertex_data> volume(PolyVox::Region{ { -1, 0, 0 }, { 2, 1, 1 } });
			volume.setVoxel(-1, 0, 0, { glm::vec3{ 1.f, 0.f, 0.f } });
			volume.setVoxel(0, 0, 0, { glm::vec3{ 0.f, 1.f, 0.f } });
			volume.setVoxel(1, 0, 0, { glm::vec3{ 0.f, 0.f, 1.f } });

			return kreogl::polyvox::load_model(volume);
		}();
		return model;
	}

	// Plane used as the "floor"
	static const kreogl::model & get_plane_model() noexcept {
		static const auto model = [] {
			constexpr auto size = 50;
			PolyVox::RawVolume<vertex_data> volume(PolyVox::Region{ { -size, 0, -size }, { size + 1, 1, size + 1 } });
			for (int x = -size; x < size; ++x)
				for (int z = -size; z < size; ++z)
					volume.setVoxel({ x, 0, z }, { glm::vec3{ 1.f } });

			return kreogl::polyvox::load_model(volume);
		}();
		return model;
	}

	// Creates a large box
	static const kreogl::model & get_box_model() noexcept {
		static const auto model = [] {
			constexpr auto size = 12;
			PolyVox::RawVolume<vertex_data> volume(PolyVox::Region{ { -size, -size, -size }, { size + 1, size + 1, size + 1 } });
			for (int x = -size; x <= size; ++x)
				for (int y = -size; y <= size; ++y) {
					// bottom
					volume.setVoxel({ x, -size, y }, { glm::vec3{ 1.f } });
					// top
					volume.setVoxel({ x, size, y }, { glm::vec3{ 1.f } });
					// front
					volume.setVoxel({ x, y, -size }, { glm::vec3{ 1.f } });
					// back
					volume.setVoxel({ x, y, size }, { glm::vec3{ 1.f } });
					// left
					volume.setVoxel({ -size, x, y }, { glm::vec3{ 1.f } });
					// right
					volume.setVoxel({ size, x, y }, { glm::vec3{ 1.f } });
				}

			return kreogl::polyvox::load_model(volume);
		}();
		return model;
	}
}

// Functions that create the static scenery. These hold the objects and lights as `static`, as they must be kept alive
namespace scene {
	// Creates a large box containing a spot light and a few blocks casting shadows on its walls
	static void create_point_light_scene(kreogl::world & world, const glm::vec3 & position) noexcept {
		const auto base_transform = glm::translate(glm::mat4(1.f), position);

		const auto create_block = [&base_transform](const glm::vec3 & position) noexcept {
			return kreogl::object{
				.model = &voxel_models::get_block_model(),
				.transform = glm::translate(base_transform, position)
			};
		};

		static const kreogl::object blocks[] = {
			create_block({ 0.f, 0.f, 5.f }), // front
			create_block({ 0.f, 0.f, -5.f }), // back
			create_block({ 0.f, 5.f, 0.f }), // top
			create_block({ 0.f, -5.f, 0.f }), // bottom
			create_block({ 5.f, 0.f, 0.f }), // left
			create_block({ -5.f, 0.f, 0.f }), // right
		};

		for (const auto & block : blocks)
			world.add(block);

		static const kreogl::object box{
			.model = &voxel_models::get_box_model(),
			.transform = glm::translate(base_transform, glm::vec3{ -12, -12, -12 })
		};
		world.add(box);

		static kreogl::point_light point_light{};
		point_light.position = position;
		point_light.volumetric_lighting = kreogl::volumetric_lighting_params{};
		world.add(point_light);
	}

	// Creates a "field" of blocks slightly above the bottom plane
	static constexpr auto s_block_field_side = 5;
	static void create_block_field_scene(kreogl::world & world, const glm::vec3 & position) noexcept {
		const auto base_transform = glm::translate(glm::mat4(1.f), position);

		static kreogl::object blocks[s_block_field_side][s_block_field_side];
		for (size_t x = 0; x < s_block_field_side; ++x)
			for (size_t z = 0; z < s_block_field_side; ++z) {
				auto & block = blocks[x][z];
				block.model = &voxel_models::get_block_model();
				block.transform = glm::translate(base_transform, glm::vec3{ x * 5.f, -45.f, z * 5.f });
				world.add(block);
			}
	}

	static void create_scene(kreogl::world & world) noexcept {
		create_point_light_scene(world, glm::vec3{ 0.f });
		create_block_field_scene(world, glm::vec3{ -25.f, 0.f, -25.f });

		// Bottom plane
		static const kreogl::object plane{
			.model = &voxel_models::get_plane_model(),
			.transform = glm::translate(glm::mat4(1.f), glm::vec3{ -50, -50, -50 })
		};
		world.add(plane);

		// Sun
		static kreogl::directional_light light;
		light.direction = { -1.f, -1.f, -1.f };
		// Enable volumetric lighting with default params
		light.volumetric_lighting = kreogl::volumetric_lighting_params{};
		world.add(light);

		// Spotlight lighting the dark corner of the "point light scene" box
		static kreogl::spot_light spot_light{
			.direction = { 0.7f, 1.f, 0.8f }
		};
		spot_light.position = { -15, -19, -16 };
		spot_light.shadow_map_min_bias = .0001f;
		spot_light.shadow_map_max_bias = .001f;
		spot_light.volumetric_lighting = kreogl::volumetric_lighting_params{};
		world.add(spot_light);

		// Block slightly in front of the spotlight, to cast a shadow
		static kreogl::object spot_light_block{
			.model = &voxel_models::get_block_model(),
			.transform = glm::translate(glm::mat4(1.f), spot_light.position + spot_light.direction * 2.f)
		};
		world.add(spot_light_block);
	}
}

int main(int ac, const char ** av) {
	// go to executable directory to be near "resources"
	const auto bin_dir = std::filesystem::path(av[0]).parent_path();
	if (exists(bin_dir))
		std::filesystem::current_path(bin_dir);

	kreogl::window window;
	input::setup_input(window);
	window.get_default_camera().set_position({ 0.f, 0.f, -5.f });

	kreogl::world world;

	const kreogl::skybox_texture skybox_texture{
		"resources/skybox/left.jpg",
		"resources/skybox/right.jpg",
		"resources/skybox/top.jpg",
		"resources/skybox/bottom.jpg",
		"resources/skybox/front.jpg",
		"resources/skybox/back.jpg",
	};
	world.skybox.texture = &skybox_texture;
	scene::create_scene(world);

	const auto funnyman_model = kreogl::assimp::load_animated_model("resources/funnyman/funnyman.fbx");
	assert(funnyman_model && funnyman_model->animations.size() == 1);

	const auto anim_file = kreogl::assimp::load_animation_file("resources/funnyman/animations/dancing.fbx");
	assert(anim_file && anim_file->animations.size() == 1);

	// Mesh dancing in the "point light scene", using the animation from the dancing.fbx
	kreogl::animated_object funnyman_in_point_light;
	{
		funnyman_in_point_light.model = funnyman_model.get();
		funnyman_in_point_light.transform = glm::translate(glm::mat4{ 1.f }, glm::vec3{ -2.5f, -2.5f, -2.5f });
		funnyman_in_point_light.animation = kreogl::animation{
			.model = anim_file->animations[0].get(),
			.loop = true
		};
		world.add(funnyman_in_point_light);
	}

	// Meshes dancing in the "block field scene", using the animation pre-baked in funnyman.fbx
	kreogl::animated_object funnymen_in_field[scene::s_block_field_side][scene::s_block_field_side];
	{
		const auto base_transform = glm::translate(glm::mat4(1.f), glm::vec3{ -25.f, 0.f, -25.f });
		for (size_t x = 0; x < scene::s_block_field_side; ++x)
			for (size_t z = 0; z < scene::s_block_field_side; ++z) {
				auto & funnyMan = funnymen_in_field[x][z];
				funnyMan.model = funnyman_model.get();
				funnyMan.transform = glm::translate(base_transform, glm::vec3{ x * 5.f, -45.f, z * 5.f });
				funnyMan.animation = kreogl::animation{
					.model = funnyman_model->animations[0].get(),
					.loop = true
				};
				world.add(funnyMan);
			}
	}

	const kreogl::debug_element debug_line{
		.type = kreogl::debug_element::type::line,
		.transform = funnyman_in_point_light.transform,
		.color = glm::vec4(1.f, 0.f, 0.f, 1.f)
	};
	world.add(debug_line);

	const kreogl::debug_element debug_sphere{
		.type = kreogl::debug_element::type::sphere,
		.transform = glm::translate(funnyman_in_point_light.transform, glm::vec3{ 0.f, 0.f, 1.f }),
		.color = glm::vec4(0.f, 1.f, 0.f, 1.f)
	};
	world.add(debug_sphere);

	const kreogl::debug_element debug_box{
		.type = kreogl::debug_element::type::box,
		.transform = glm::translate(funnyman_in_point_light.transform, glm::vec3{ 0.f, 0.f, -1.f }),
		.color = glm::vec4(0.f, 0.f, 1.f, 1.f)
	};
	world.add(debug_box);

	const kreogl::texture_data koala_texture_data("resources/sprites/koala.png");
	const auto koala_texture = koala_texture_data.load_to_texture();

	kreogl::sprite_2d sprite_2d;
	sprite_2d.tex = &koala_texture;
	sprite_2d.transform = glm::translate(glm::mat4{ 1.f }, glm::vec3{ -.95f, -.9f, -1.f }); // Bottom left corner
	sprite_2d.transform = glm::scale(sprite_2d.transform, glm::vec3{ .1f / window.get_default_camera().get_viewport().get_aspect_ratio(), .1f, 0.f });
	world.add(sprite_2d);

	kreogl::text_2d text_2d;
	text_2d.value = "Kreogl example";
	text_2d.font = "resources/fonts/arial.ttf";
	text_2d.transform = glm::translate(glm::mat4{ 1.f }, glm::vec3{ -.9f, -.9f, -1.f }); // Bottom left, to the right of sprite_2d
	text_2d.transform = glm::scale(text_2d.transform, glm::vec3{ .1f, .1f, 0.f });
	text_2d.alignment = kreogl::text::alignment_type::left;
	world.add(text_2d);

	kreogl::text_3d text_3d;
	text_3d.value = "3D text example";
	text_3d.font = "resources/fonts/arial.ttf";
	text_3d.transform = glm::translate(funnyman_in_point_light.transform, glm::vec3{ 0.f, -1.f, 0.f });
	text_3d.color = glm::vec4(1.f, 1.f, 0.f, .5f);
	world.add(text_3d);

	kreogl::sprite_3d sprite_3d;
	sprite_3d.tex = &koala_texture;
	sprite_3d.transform = glm::translate(text_3d.transform, glm::vec3{ 0.f, -1.5f, 0.f });
	sprite_3d.color = text_3d.color;
	world.add(sprite_3d);

	auto previous_time = std::chrono::system_clock::now();
	while (!window.should_close()) {
		const auto now = std::chrono::system_clock::now();
		const auto delta_time = (float)std::chrono::duration_cast<std::chrono::milliseconds>(now - previous_time).count() / 1000.f;

		input::process_input(window, delta_time);
		camera_rotation::rotate_fixed_cameras(window);

		funnyman_in_point_light.tick_animation(delta_time);
		for (auto & line : funnymen_in_field)
			for (auto & funnyMan : line)
				funnyMan.tick_animation(delta_time);

		window.poll_events();
		window.draw(
			world
#ifndef KREOGL_DEFAULT_SHADERS
			,
			kreogl::shader_pipeline{}
#endif
		);
		window.display();

		previous_time = now;

		KREOGL_PROFILING_FRAME;
	}

	return 0;
}