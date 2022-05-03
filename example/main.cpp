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
        if (action == GLFW_PRESS)
            s_keysPressed[key] = true;
        else if (action == GLFW_RELEASE)
            s_keysPressed[key] = false;
    });
}

static const kreogl::Object & createBlock() noexcept {
    PolyVox::RawVolume<VertexData> volume(PolyVox::Region{ { -1, 0, 0 }, { 2, 1, 1 } });
    volume.setVoxel(-1, 0, 0, { glm::vec3(1.f, 0.f, 0.f) });
    volume.setVoxel(0, 0, 0, { glm::vec3(0.f, 1.f, 0.f) });
    volume.setVoxel(1, 0, 0, { glm::vec3(0.f, 0.f, 1.f) });

    static const auto model = kreogl::PolyVox::loadModel(volume);
    static const kreogl::Object object {
        .model = &model,
        .transform = glm::translate(glm::mat4(1.f), { 0.f, 0.f, 5.f })
    };
    return object;
}

static const kreogl::Object & createPlane() noexcept {
    PolyVox::RawVolume<VertexData> volume(PolyVox::Region{ { -50, 0, -50 }, { 51, 1, 51 } });
    for (int x = -50; x < 50; ++x)
        for (int z = -50; z < 50; ++z)
            volume.setVoxel({ x, 0, z }, { glm::vec3(1.f) });

    static const auto model = kreogl::PolyVox::loadModel(volume);
    static const kreogl::Object object {
        .model = &model,
        .transform = glm::translate(glm::mat4(1.f), glm::vec3(-50.f, -2.f, -50.f))
    };
    return object;
}

static void createScene(kreogl::World & world) noexcept {
    world.add(createBlock());
    world.add(createPlane());

    static const kreogl::DirectionalLight light{
        .direction = { -1.f, -1.f, -1.f }
    };
    world.add(light);
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
    camera.setPosition(position);
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

        window.pollEvents();
        window.draw(world);
        window.display();

        previousTime = now;
    }

    return 0;
}