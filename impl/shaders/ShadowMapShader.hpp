#pragma once

#include "Shader.hpp"
#include "lights/DirectionalLight.hpp"
#include "lights/SpotLight.hpp"
#include "impl/DrawParams.hpp"

namespace kreogl {
    class ShadowMapShader : public Shader {
    public:
        void draw(const DirectionalLight & light, const DrawParams & params) noexcept;
        void draw(const SpotLight & light, const DrawParams & params) noexcept;

    protected:
        virtual void drawToTexture(GLuint texture, const glm::mat4 & lightSpaceMatrix, const DrawParams & params) noexcept {}

    private:
        void draw(const DrawParams & params) noexcept override {}

        template<typename T, typename Func>
        void drawImpl(T & shadowMap, Func && draw, const DrawParams & params) noexcept;
    };
}