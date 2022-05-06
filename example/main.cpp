#include "kreogl/Window.hpp"
#include "kreogl/World.hpp"

#include "PolyVox/RawVolume.h"
#include "kreogl/model/PolyVox/PolyVoxModel.hpp"

#include <GLFW/glfw3.h>
#include <glm/gtx/rotate_vector.hpp>

struct VertexData {
    glm::vec3 color;

    bool operator==(size_t i) const {
        return color == glm::vec3(0.f);
    }

    bool operator>(size_t i) const {
        return color != glm::vec3(0.f);
    }

    bool operator==(const VertexData & rhs) const {
        return color == rhs.color;
    }
};

static kreogl::Window * s_window = nullptr;
static std::unordered_map<int, bool> s_keysPressed;
static bool s_rotatingFixedCameras = true;

static constexpr auto MOUSE_SENSITIVITY = .01f;
static constexpr auto MOVEMENT_SPEED = 5.f;

struct CameraVectors {
    glm::vec3 front;
    glm::vec3 right;
    glm::vec3 up;
};

static CameraVectors getCameraVectors(const kreogl::Camera & camera) noexcept {
    const auto front = camera.getDirection();
    const auto right = glm::cross(front, { 0.f, 1.f, 0.f });
    const auto up = glm::cross(right, front);
    return { front, right, up };
}

static void setupInput(kreogl::Window & window) noexcept {
    s_window = &window;

    glfwSetInputMode(window.getGLFWwindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSetCursorPosCallback(window.getGLFWwindow(), [](GLFWwindow * window, double xPos, double yPos) noexcept {
        static glm::vec2 lastPos = { (float)xPos, (float)yPos };

        const glm::vec2 newPos = { (float)xPos, (float)yPos };
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
            s_rotatingFixedCameras = false; // Disable automatic camera changes if user wants to move around
            s_keysPressed[key] = true;
        }
        else if (action == GLFW_RELEASE)
            s_keysPressed[key] = false;
    });
}

static const kreogl::Model & getBlockModel() noexcept {
    static const auto model = [] {
        PolyVox::RawVolume<VertexData> volume(PolyVox::Region{{-1, 0, 0},
                                                              {2,  1, 1}});
        volume.setVoxel(-1, 0, 0, {glm::vec3(1.f, 0.f, 0.f)});
        volume.setVoxel(0, 0, 0, {glm::vec3(0.f, 1.f, 0.f)});
        volume.setVoxel(1, 0, 0, {glm::vec3(0.f, 0.f, 1.f)});

        return kreogl::PolyVox::loadModel(volume);
    }();
    return model;
}

static const kreogl::Model & getPlaneModel() noexcept {
    static const auto model = [] {
        constexpr auto size = 50;
        PolyVox::RawVolume<VertexData> volume(PolyVox::Region{ { -size, 0, -size }, { size + 1, 1, size + 1 } });
        for (int x = -size; x < size; ++x)
            for (int z = -size; z < size; ++z)
                volume.setVoxel({ x, 0, z }, { glm::vec3(1.f) });

        return kreogl::PolyVox::loadModel(volume);
    }();
    return model;
}

static const kreogl::Model & getBoxModel() noexcept {
    static const auto model = [] {
        constexpr auto size = 12;
        PolyVox::RawVolume<VertexData> volume(PolyVox::Region{ { -size, -size, -size }, { size + 1, size + 1, size + 1 } });
        for (int x = -size; x <= size; ++x)
            for (int y = -size; y <= size; ++y) {
                // bottom
                volume.setVoxel({ x, -size, y }, { glm::vec3(1.f) });
                // top
                volume.setVoxel({ x, size, y }, { glm::vec3(1.f) });
                // front
                volume.setVoxel({ x, y, -size }, { glm::vec3(1.f) });
                // back
                volume.setVoxel({ x, y, size }, { glm::vec3(1.f) });
                // left
                volume.setVoxel({ -size, x, y }, { glm::vec3(1.f) });
                // right
                volume.setVoxel({ size, x, y }, { glm::vec3(1.f) });
            }

        return kreogl::PolyVox::loadModel(volume);
    }();
    return model;
}

static void createPointLightScene(kreogl::World & world, const glm::vec3 & position) noexcept {
    const auto baseTransform = glm::translate(glm::mat4(1.f), position);
    static const kreogl::Object frontBlock {
        .model = &getBlockModel(),
        .transform = glm::translate(baseTransform, { 0.f, 0.f, 5.f })
    };
    world.add(frontBlock);
    static const kreogl::Object backBlock {
        .model = &getBlockModel(),
        .transform = glm::translate(baseTransform, { 0.f, 0.f, -5.f })
    };
    world.add(backBlock);
    static const kreogl::Object topBlock {
        .model = &getBlockModel(),
        .transform = glm::translate(baseTransform, { 0.f, 5.f, 0.f })
    };
    world.add(topBlock);
    static const kreogl::Object bottomBlock {
        .model = &getBlockModel(),
        .transform = glm::translate(baseTransform, { 0.f, -5.f, 0.f })
    };
    world.add(bottomBlock);
    static const kreogl::Object leftBlock {
        .model = &getBlockModel(),
        .transform = glm::translate(baseTransform, { 5.f, 0.f, 0.f })
    };
    world.add(leftBlock);
    static const kreogl::Object rightBlock {
        .model = &getBlockModel(),
        .transform = glm::translate(baseTransform, { -5.f, 0.f, 0.f })
    };
    world.add(rightBlock);

    static const kreogl::Object box {
        .model = &getBoxModel(),
        .transform = glm::translate(baseTransform, glm::vec3(-12, -12, -12))
    };
    world.add(box);

    static const kreogl::PointLight pointLight{};
    world.add(pointLight);
}

static void createBlockFieldScene(kreogl::World & world, const glm::vec3 & position) noexcept {
    const auto baseTransform = glm::translate(glm::mat4(1.f), position);

    static kreogl::Object blocks[10][10];
    for (size_t x = 0; x < 10; ++x)
        for (size_t z = 0; z < 10; ++z) {
            auto & block = blocks[x][z];
            block.model = &getBlockModel();
            block.transform = glm::translate(baseTransform, glm::vec3(x * 5.f, -45.f, z * 5.f));
            world.add(block);
        }
}

static void createScene(kreogl::World & world) noexcept {
    createPointLightScene(world, glm::vec3(0.f));
    createBlockFieldScene(world, glm::vec3(-25.f, 0.f, -25.f));

    static const kreogl::Object plane {
        .model = &getPlaneModel(),
        .transform = glm::translate(glm::mat4(1.f), glm::vec3(-50, -50, -50))
    };
    world.add(plane);

    static const kreogl::DirectionalLight light{
        .direction = { -1.f, -1.f, -1.f }
    };
    world.add(light);

    static kreogl::SpotLight spotLight{
        .direction = { 0.f, 1.f, -1.f }
    };
    spotLight.position = { -15.f, -15.f, -15.f };
    world.add(spotLight);
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
        s_rotatingFixedCameras = true;

    camera.setPosition(position);
}

static void rotateFixedCameras(kreogl::Window & window) noexcept {
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
            .position = { 7.5f, -43.f, -8.f },
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

int main() {
    kreogl::Window window({});
    setupInput(window);
    window.getDefaultCamera().setPosition({ 0.f, 0.f, -5.f });

    kreogl::World world;
    createScene(world);

    auto previousTime = std::chrono::system_clock::now();
    while (!window.shouldClose()) {
        const auto now = std::chrono::system_clock::now();
        const auto deltaTime = (float)std::chrono::duration_cast<std::chrono::milliseconds>(now - previousTime).count() / 1000.f;

        processInput(window, deltaTime);
        if (s_rotatingFixedCameras)
            rotateFixedCameras(window);

        window.pollEvents();
        window.draw(world);
        window.display();

        previousTime = now;
    }

    return 0;
}