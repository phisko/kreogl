// stl
#include <filesystem>

// kreogl
#include "kreogl/window.hpp"
#include "kreogl/world.hpp"
#include "kreogl/animation/animated_object.hpp"
#include "kreogl/loaders/assimp/assimp.hpp"

int main(int ac, const char ** av) {
	// Go to executable directory to be near "resources"
	const auto bin_dir = std::filesystem::path(av[0]).parent_path();
	if (exists(bin_dir))
		std::filesystem::current_path(bin_dir);

	kreogl::window window; // Create a window
	window.get_default_camera().set_position({ 0.f, 0.f, -5.f }); // Move the camera back to see the centered scene

	kreogl::world world; // The world that will be used to draw into the window

	// Load the skybox
	const kreogl::skybox_texture skybox_texture{
		"resources/skybox/left.jpg",
		"resources/skybox/right.jpg",
		"resources/skybox/top.jpg",
		"resources/skybox/bottom.jpg",
		"resources/skybox/front.jpg",
		"resources/skybox/back.jpg",
	};
	world.skybox.texture = &skybox_texture; // Add it to the world

	kreogl::directional_light light; // Create a light
	world.add(light); // Add it to the world
	light.direction = { 0.f, -1.f, -1.f };
	light.cast_shadows = false; // Disable shadows for our scene

	const auto model = kreogl::assimp::load_animated_model("resources/funnyman/funnyman.fbx"); // Load a 3D model
	assert(model && model->animations.size() == 1);

	kreogl::animated_object object; // Create an object
	object.model = model.get(); // Base it on the loaded 3D model
	object.transform = glm::translate(glm::mat4{ 1.f }, glm::vec3{ 0.f, -2.5f, 5.f }); // Move it forward and down a bit
	object.transform = glm::rotate(object.transform, glm::pi<float>(), glm::vec3{ 0.f, 1.f, 0.f }); // Rotate it to face the camera
	object.animation = kreogl::animation{
		// Play an animation
		.model = model->animations[0].get(), // Use the animation that was baked into the 3D model
		.loop = true
	};
	world.add(object); // Add the object to the world

	// Main loop
	auto previous_time = std::chrono::system_clock::now();
	while (!window.should_close()) {
		const auto now = std::chrono::system_clock::now();
		const auto delta_time = float(std::chrono::duration_cast<std::chrono::milliseconds>(now - previous_time).count()) / 1000.f;
		previous_time = now;

		object.tick_animation(delta_time); // Play the object's animation

		window.poll_events(); // Process input
		window.draw( // Draw the world into the window
			world
#ifndef KREOGL_DEFAULT_SHADERS // This isn't required in client code that uses the default shaders
			,
			kreogl::shader_pipeline{} // If default shaders aren't enabled, use an empty pipeline
#endif
		);
		window.display(); // Present the new window contents

		KREOGL_PROFILING_FRAME; // Notify the profiling system that a frame has ended
	}
}