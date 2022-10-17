// stl
#include <filesystem>

// kreogl
#include "kreogl/Window.hpp"
#include "kreogl/World.hpp"
#include "kreogl/animation/AnimatedObject.hpp"
#include "kreogl/loaders/assimp/AssImp.hpp"
#include "kreogl/loaders/polyvox/PolyVox.hpp"

// polyvox
#include "PolyVox/RawVolume.h"

// GLFW
#include <GLFW/glfw3.h>
#include <glm/gtx/rotate_vector.hpp>

namespace cameraRotation {
    static bool s_rotatingFixedCameras = true;

    static void rotateFixedCameras(kreogl::Window & window) noexcept {
        if (!s_rotatingFixedCameras)
            return;

        struct Params {
            glm::vec3 position;
            glm::vec3 direction;
        };

        static const Params params[] = {
            { // Inside the point light demo
                .position = { -9.f, 7.f, -7.5f },
                .direction = { 1.3f, -1.f, 0.6f }
            },
            { // Global view
                .position = { 24.f, -7.f, -23.f },
                .direction = { -1.2f, -0.8f, 0.f }
            },
            { // Close up of blocks
                .position = { 0.f, -40.f, -20.f },
                .direction = { -1.f, -0.5f, 0.6f }
            }
        };
        static size_t current = 0;

        static const auto timeBetweenChanges = std::chrono::seconds(3);
        static auto lastChangeTime = std::chrono::system_clock::now() - timeBetweenChanges * 2; // make sure we trigger a change on first frame

        const auto now = std::chrono::system_clock::now();
        if (now - lastChangeTime > timeBetweenChanges) {
            const auto & currentParams = params[current];
            auto & camera = window.getDefaultCamera();
            camera.setPosition(currentParams.position);
            camera.setDirection(currentParams.direction);

            current = (current + 1) % std::extent_v<decltype(params)>;
            lastChangeTime = now;
        }
    }
}

namespace input {
    static kreogl::Window * s_window = nullptr;
    static std::unordered_map<int, bool> s_keysPressed;

    static constexpr auto MOUSE_SENSITIVITY = .01f;
    static constexpr auto MOVEMENT_SPEED = 5.f;

    struct CameraVectors {
        glm::vec3 front;
        glm::vec3 right;
        glm::vec3 up;
    };

    static CameraVectors getCameraVectors(const kreogl::Camera & camera) noexcept {
        const auto front = camera.getDirection();
        const auto right = glm::cross(front, {0.f, 1.f, 0.f});
        const auto up = glm::cross(right, front);
        return {front, right, up};
    }

    static void setupInput(kreogl::Window & window) noexcept {
        s_window = &window;

        glfwSetInputMode(window.getGLFWwindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        glfwSetCursorPosCallback(window.getGLFWwindow(), [](GLFWwindow * window, double xPos, double yPos) noexcept {
            static glm::vec2 lastPos = {(float) xPos, (float) yPos};

            const glm::vec2 newPos = {(float) xPos, (float) yPos};
            const glm::vec2 delta = newPos - lastPos;
            lastPos = newPos;

            auto & camera = s_window->getDefaultCamera();

            auto direction = camera.getDirection();
            const auto vectors = getCameraVectors(camera);
            direction = glm::rotate(direction, -delta.x * MOUSE_SENSITIVITY, vectors.up);
            direction = glm::rotate(direction, -delta.y * MOUSE_SENSITIVITY, vectors.right);
            camera.setDirection(direction);
        });

        glfwSetKeyCallback(window.getGLFWwindow(), [](GLFWwindow * window, int key, int scancode, int action, int mods) noexcept {
            if (action == GLFW_PRESS) {
                cameraRotation::s_rotatingFixedCameras = false; // Disable automatic camera changes if user wants to move around
                s_keysPressed[key] = true;
            } else if (action == GLFW_RELEASE)
                s_keysPressed[key] = false;
        });
    }

    static void processInput(kreogl::Window & window, float deltaTime) noexcept {
        auto & camera = window.getDefaultCamera();

        const auto vectors = getCameraVectors(camera);
        const auto velocity = MOVEMENT_SPEED * deltaTime;

        auto position = camera.getPosition();

        if (s_keysPressed['W'])
            position += vectors.front * velocity;
        if (s_keysPressed['S'])
            position -= vectors.front * velocity;
        if (s_keysPressed['A'])
            position -= vectors.right * velocity;
        if (s_keysPressed['D'])
            position += vectors.right * velocity;

        if (s_keysPressed[GLFW_KEY_LEFT_SHIFT])
            position += vectors.up * velocity;
        if (s_keysPressed[GLFW_KEY_LEFT_CONTROL])
            position -= vectors.up * velocity;

        if (s_keysPressed['R'])
            cameraRotation::s_rotatingFixedCameras = true;

        camera.setPosition(position);
    }


}

namespace voxelModels {
    struct VertexData {
        glm::vec3 color;

        bool operator==(size_t i) const {
            return color == glm::vec3{ 0.f };
        }

        bool operator>(size_t i) const {
            return color != glm::vec3{ 0.f };
        }

        bool operator==(const VertexData & rhs) const {
            return color == rhs.color;
        }
    };

    static const kreogl::Model & getBlockModel() noexcept {
        static const auto model = [] {
            PolyVox::RawVolume<VertexData> volume(PolyVox::Region{{-1, 0, 0},
                                                                  {2,  1, 1}});
            volume.setVoxel(-1, 0, 0, { glm::vec3 { 1.f, 0.f, 0.f } });
            volume.setVoxel(0, 0, 0, { glm::vec3{ 0.f, 1.f, 0.f } });
            volume.setVoxel(1, 0, 0, { glm::vec3{ 0.f, 0.f, 1.f } });

            return kreogl::PolyVox::loadModel(volume);
        }();
        return model;
    }

    static const kreogl::Model & getPlaneModel() noexcept {
        static const auto model = [] {
            constexpr auto size = 50;
            PolyVox::RawVolume<VertexData> volume(PolyVox::Region{{-size,    0, -size},
                                                                  {size + 1, 1, size + 1}});
            for (int x = -size; x < size; ++x)
                for (int z = -size; z < size; ++z)
                    volume.setVoxel({x, 0, z}, { glm::vec3{ 1.f } });

            return kreogl::PolyVox::loadModel(volume);
        }();
        return model;
    }

    static const kreogl::Model & getBoxModel() noexcept {
        static const auto model = [] {
            constexpr auto size = 12;
            PolyVox::RawVolume<VertexData> volume(PolyVox::Region{{-size,    -size,    -size},
                                                                  {size + 1, size + 1, size + 1}});
            for (int x = -size; x <= size; ++x)
                for (int y = -size; y <= size; ++y) {
                    // bottom
                    volume.setVoxel({x, -size, y}, { glm::vec3{ 1.f } });
                    // top
                    volume.setVoxel({x, size, y}, { glm::vec3{ 1.f } });
                    // front
                    volume.setVoxel({x, y, -size}, { glm::vec3{ 1.f } });
                    // back
                    volume.setVoxel({x, y, size}, { glm::vec3{ 1.f } });
                    // left
                    volume.setVoxel({-size, x, y}, { glm::vec3{ 1.f } });
                    // right
                    volume.setVoxel({size, x, y}, { glm::vec3{ 1.f } });
                }

            return kreogl::PolyVox::loadModel(volume);
        }();
        return model;
    }
}

namespace scene {
    // Creates a large box containing a spot light and a few blocks casting shadows on its walls
    static void createPointLightScene(kreogl::World & world, const glm::vec3 & position) noexcept {
        const auto baseTransform = glm::translate(glm::mat4(1.f), position);

        const auto createBlock = [&baseTransform](const glm::vec3 & position) noexcept {
            return kreogl::Object{
                .model = &voxelModels::getBlockModel(),
                .transform = glm::translate(baseTransform, position)
            };
        };

        static const kreogl::Object blocks[] = {
            createBlock({0.f, 0.f, 5.f}), // front
            createBlock({0.f, 0.f, -5.f}), // back
            createBlock({0.f, 5.f, 0.f}), // top
            createBlock({0.f, -5.f, 0.f}), // bottom
            createBlock({5.f, 0.f, 0.f}), // left
            createBlock({-5.f, 0.f, 0.f}), // right
        };

        for (const auto & block: blocks)
            world.add(block);

        static const kreogl::Object box{
            .model = &voxelModels::getBoxModel(),
            .transform = glm::translate(baseTransform, glm::vec3{ -12, -12, -12 })
        };
        world.add(box);

        static kreogl::PointLight pointLight{};
        pointLight.position = position;
        pointLight.volumetricLighting = kreogl::VolumetricLightingParams{};
        world.add(pointLight);
    }

    // Creates a "field" of blocks slightly above the bottom plane
    static constexpr auto s_blockFieldSide = 5;
    static void createBlockFieldScene(kreogl::World & world, const glm::vec3 & position) noexcept {
        const auto baseTransform = glm::translate(glm::mat4(1.f), position);

        static kreogl::Object blocks[s_blockFieldSide][s_blockFieldSide];
        for (size_t x = 0; x < s_blockFieldSide; ++x)
            for (size_t z = 0; z < s_blockFieldSide; ++z) {
                auto & block = blocks[x][z];
                block.model = &voxelModels::getBlockModel();
                block.transform = glm::translate(baseTransform, glm::vec3{ x * 5.f, -45.f, z * 5.f });
                world.add(block);
            }
    }

    static void createScene(kreogl::World & world) noexcept {
        createPointLightScene(world, glm::vec3{ 0.f });
        createBlockFieldScene(world, glm::vec3{ -25.f, 0.f, -25.f });

        // Bottom plane
        static const kreogl::Object plane{
            .model = &voxelModels::getPlaneModel(),
            .transform = glm::translate(glm::mat4(1.f), glm::vec3{ -50, -50, -50 })
        };
        world.add(plane);

        // Sun
        static kreogl::DirectionalLight light;
        light.direction = {-1.f, -1.f, -1.f};
        // Enable volumetric lighting with default params
        light.volumetricLighting = kreogl::VolumetricLightingParams{};
        world.add(light);

        // Spotlight lighting the dark corner of the "point light scene" box
        static kreogl::SpotLight spotLight{
            .direction = {0.7f, 1.f, 0.8f}
        };
        spotLight.position = {-15, -19, -16};
        spotLight.shadowMapMinBias = .0001f;
        spotLight.shadowMapMaxBias = .001f;
        spotLight.volumetricLighting = kreogl::VolumetricLightingParams{};
        world.add(spotLight);

        // Block slightly in front of the spotlight, to cast a shadow
        static kreogl::Object spotLightBlock{
            .model = &voxelModels::getBlockModel(),
            .transform = glm::translate(glm::mat4(1.f), spotLight.position + spotLight.direction * 2.f)
        };
        world.add(spotLightBlock);
    }
}

int main(int ac, const char ** av) {
    // go to executable directory to be near "resources"
    const auto binDir = std::filesystem::path(av[0]).parent_path();
    if (exists(binDir))
        std::filesystem::current_path(binDir);

    kreogl::Window window;
    input::setupInput(window);
    window.getDefaultCamera().setPosition({ 0.f, 0.f, -5.f });

    kreogl::World world;
    world.skybox.texture = kreogl::SkyboxTexture{
        "resources/skybox/left.jpg",
        "resources/skybox/right.jpg",
        "resources/skybox/top.jpg",
        "resources/skybox/bottom.jpg",
        "resources/skybox/front.jpg",
        "resources/skybox/back.jpg",
    };
    scene::createScene(world);

    const auto funnyManModel = kreogl::AssImp::loadAnimatedModel("resources/funnyman/funnyman.fbx");
    assert(funnyManModel && funnyManModel->animations.size() == 1);

    const auto animFile = kreogl::AssImp::loadAnimationFile("resources/funnyman/animations/dancing.fbx");
    assert(animFile && animFile->animations.size() == 1);

    // Mesh dancing in the "point light scene"
    kreogl::AnimatedObject funnyManInPointLight; {
        funnyManInPointLight.model = funnyManModel.get();
        funnyManInPointLight.transform = glm::translate(glm::mat4{1.f}, glm::vec3{ -2.5f, -2.5f, -2.5f });
        funnyManInPointLight.animation = kreogl::Animation{
            .model = animFile->animations[0].get(),
            .loop = true
        };
        world.add(funnyManInPointLight);
    }

    // Meshes dancing in the "block field scene"
    kreogl::AnimatedObject funnyMenInField[scene::s_blockFieldSide][scene::s_blockFieldSide]; {
        const auto baseTransform = glm::translate(glm::mat4(1.f), glm::vec3{ -25.f, 0.f, -25.f });
        for (size_t x = 0; x < scene::s_blockFieldSide; ++x)
            for (size_t z = 0; z < scene::s_blockFieldSide; ++z) {
                auto & funnyMan = funnyMenInField[x][z];
                funnyMan.model = funnyManModel.get();
                funnyMan.transform = glm::translate(baseTransform, glm::vec3{ x * 5.f, -45.f, z * 5.f });
                funnyMan.animation = kreogl::Animation{
                    .model = funnyManModel->animations[0].get(),
                    .loop = true
                };
                world.add(funnyMan);
            }
    }

    const kreogl::DebugElement debugLine {
        .type = kreogl::DebugElement::Type::Line,
        .transform = funnyManInPointLight.transform,
        .color = glm::vec4(1.f, 0.f, 0.f, 1.f)
    };
    world.add(debugLine);

    const kreogl::DebugElement debugSphere {
        .type = kreogl::DebugElement::Type::Sphere,
        .transform = glm::translate(funnyManInPointLight.transform, glm::vec3{ 0.f, 0.f, 1.f }),
        .color = glm::vec4(0.f, 1.f, 0.f, 1.f)
    };
    world.add(debugSphere);

    const kreogl::DebugElement debugBox {
        .type = kreogl::DebugElement::Type::Box,
        .transform = glm::translate(funnyManInPointLight.transform, glm::vec3{ 0.f, 0.f, -1.f }),
        .color = glm::vec4(0.f, 0.f, 1.f, 1.f)
    };
    world.add(debugBox);

    kreogl::Text2D text2D;
    text2D.text = "Kreogl example";
    text2D.font = "resources/fonts/arial.ttf";
    text2D.transform = glm::translate(glm::mat4{ 1.f }, glm::vec3{ 1.f, -.9f, -1.f });
    text2D.transform = glm::scale(text2D.transform, glm::vec3{ .1f, .1f, 0.f });
    text2D.alignment = kreogl::Text::Alignment::Right;
    world.add(text2D);

    kreogl::Text3D text3D;
    text3D.text = "3D text example";
    text3D.font = "resources/fonts/arial.ttf";
    text3D.transform = glm::translate(funnyManInPointLight.transform, glm::vec3{ 0.f, -1.f, 0.f });
    text3D.color = glm::vec4(1.f, 1.f, 0.f, .5f);
    world.add(text3D);

    auto previousTime = std::chrono::system_clock::now();
    while (!window.shouldClose()) {
        const auto now = std::chrono::system_clock::now();
        const auto deltaTime = (float)std::chrono::duration_cast<std::chrono::milliseconds>(now - previousTime).count() / 1000.f;

        input::processInput(window, deltaTime);
        cameraRotation::rotateFixedCameras(window);

        funnyManInPointLight.tickAnimation(deltaTime);
        for (auto & line : funnyMenInField)
            for (auto & funnyMan : line)
                funnyMan.tickAnimation(deltaTime);

        window.pollEvents();
        window.draw(
            world
#ifndef KREOGL_DEFAULT_SHADERS
            , kreogl::ShaderPipeline{}
#endif
        );
        window.display();

        previousTime = now;
    }

    return 0;
}