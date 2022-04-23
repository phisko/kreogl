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
        direction = glm::rotate(direction, -delta.x * MOUSE_SENSITIVITY, glm::vec3(0.f, 1.f, 0.f));
        direction = glm::rotate(direction, delta.y * MOUSE_SENSITIVITY, glm::vec3(1.f, 0.f, 0.f));
        camera.setDirection(direction);
    });

    glfwSetKeyCallback(window.getGLFWwindow(), [](GLFWwindow * window, int key, int scancode, int action, int mods) noexcept {
        if (action == GLFW_PRESS)
            s_keysPressed[key] = true;
        else if (action == GLFW_RELEASE)
            s_keysPressed[key] = false;
    });
}

static void createScene(kreogl::World & world) noexcept {
    PolyVox::RawVolume<VertexData> volume(PolyVox::Region{ { 0, 0, 0 }, { 42, 42, 42 } });
    volume.setVoxel(1, 1, 1, { glm::vec3(1.f, 0.f, 0.f) });
    volume.setVoxel(1, 1, 2, { glm::vec3(0.f, 1.f, 0.f) });
    volume.setVoxel(1, 1, 3, { glm::vec3(0.f, 0.f, 1.f) });

    static const auto model = kreogl::PolyVox::loadModel(volume);

    static kreogl::Object object;
    object.model = &model;
    object.transform = glm::translate(object.transform, glm::vec3(0.f, -2.f, 0.f));
    world.add(object);

    static const kreogl::DirectionalLight light{
        .direction = { -1.f, -1.f, 0.f }
    };
    world.add(light);
}

static void processInput(kreogl::Window & window, float deltaTime) noexcept {
    auto & camera = window.getDefaultCamera();

    const auto front = camera.getDirection();
    const auto right = glm::cross(front, { 0.f, 1.f, 0.f });
    const auto up = glm::cross(right, front);

    const auto velocity = MOVEMENT_SPEED * deltaTime;

    auto position = camera.getPosition();
    if (s_keysPressed['W'])
        position += front * velocity;
    if (s_keysPressed['S'])
        position -= front * velocity;
    if (s_keysPressed['A'])
        position -= right * velocity;
    if (s_keysPressed['D'])
        position += right * velocity;
    if (s_keysPressed[GLFW_KEY_LEFT_SHIFT])
        position += up * velocity;
    if (s_keysPressed[GLFW_KEY_LEFT_CONTROL])
        position += up * velocity;
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
        previousTime = now;
    }

    return 0;
}