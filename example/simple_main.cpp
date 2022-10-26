// stl
#include <filesystem>

// kreogl
#include "kreogl/Window.hpp"
#include "kreogl/World.hpp"
#include "kreogl/animation/AnimatedObject.hpp"
#include "kreogl/loaders/assimp/AssImp.hpp"

int main(int ac, const char ** av) {
	// Go to executable directory to be near "resources"
	const auto binDir = std::filesystem::path(av[0]).parent_path();
    if (exists(binDir))
        std::filesystem::current_path(binDir);

	kreogl::Window window; // Create a window
	window.getDefaultCamera().setPosition({ 0.f, 0.f, -5.f }); // Move the camera back to see the centered scene

	kreogl::World world; // The world that will be used to draw into the window
	world.skybox.texture = kreogl::SkyboxTexture{ // Set the skybox
		"resources/skybox/left.jpg",
		"resources/skybox/right.jpg",
		"resources/skybox/top.jpg",
		"resources/skybox/bottom.jpg",
		"resources/skybox/front.jpg",
		"resources/skybox/back.jpg",
	};

	kreogl::DirectionalLight light; // Create a light
	world.add(light); // Add it to the world
	light.direction = { 0.f, -1.f, -1.f };
	light.castShadows = false; // Disable shadows for our scene

	const auto model = kreogl::AssImp::loadAnimatedModel("resources/funnyman/funnyman.fbx"); // Load a 3D model
	assert(model && model->animations.size() == 1);

	kreogl::AnimatedObject object; // Create an object
	object.model = model.get(); // Base it on the loaded 3D model
	object.transform = glm::translate(glm::mat4{1.f}, glm::vec3{ 0.f, -2.5f, 5.f }); // Move it forward and down a bit
	object.transform = glm::rotate(object.transform, glm::pi<float>(), glm::vec3{ 0.f, 1.f, 0.f }); // Rotate it to face the camera
	object.animation = kreogl::Animation{ // Play an animation
		.model = model->animations[0].get(), // Use the animation that was baked into the 3D model
		.loop = true
	};
	world.add(object); // Add the object to the world

	// Main loop
	auto previousTime = std::chrono::system_clock::now();
	while (!window.shouldClose()) {
		const auto now = std::chrono::system_clock::now();
		const auto deltaTime = float(std::chrono::duration_cast<std::chrono::milliseconds>(now - previousTime).count()) / 1000.f;
		previousTime = now;

		object.tickAnimation(deltaTime); // Play the object's animation

		window.pollEvents(); // Process input
		window.draw( // Draw the world into the window
			world
#ifndef KREOGL_DEFAULT_SHADERS // This isn't required in client code that uses the default shaders
			, kreogl::ShaderPipeline{} // If default shaders aren't enabled, use an empty pipeline
#endif
		);
		window.display(); // Present the new window contents

		KREOGL_PROFILING_FRAME; // Notify the profiling system that a frame has ended
	}
}