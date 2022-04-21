#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "impl/DrawParams.hpp"
#include "Uniform.hpp"

namespace kreogl {
    class Shader {
    public:
        virtual ~Shader() noexcept = default;

        virtual void draw(const DrawParams & params) noexcept = 0;

    protected:
        void init() noexcept; // must be called for initialization
        void addSourceFile(const char * glsl, GLenum type) noexcept;
        virtual void addSourceFiles() noexcept = 0;
        virtual std::vector<UniformBase *> getUniforms() noexcept = 0;

    protected:
        void use() noexcept;

    private:
        void link() noexcept;
        void getUniformLocations() noexcept;

    private:
        GLuint _program = -1;
        std::vector<UniformBase *> _uniforms;
    };
}