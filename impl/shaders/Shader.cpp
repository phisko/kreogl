#include "Shader.hpp"

#include <iostream>

namespace kreogl {
    void Shader::use() noexcept {
        glUseProgram(_program);
    }

    void Shader::init() noexcept {
        _program = glCreateProgram();
        addSourceFiles();
        link();
        _uniforms = getUniforms();
        getUniformLocations();
    }

    void Shader::addSourceFile(const char *glsl, GLenum type) noexcept {
        const auto shader = glCreateShader(type);
        glShaderSource(shader, 1, &glsl, nullptr);
        glCompileShader(shader);

#ifndef _NDEBUG
        {
            char buffer[512];
            glGetShaderInfoLog(shader, sizeof(buffer), nullptr, buffer);
            if (strlen(buffer) != 0) {
                std::cerr << "Error compiling program [" << typeid(this).name() << "]:" << std::endl
                          << '\t' << buffer << std::endl;
                assert(false);
            }

            GLint status;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
            assert(status == GL_TRUE);
        }
#endif

        glAttachShader(_program, shader);
    }

    void Shader::link() noexcept {
        glLinkProgram(_program);
#ifndef _NDEBUG
        {
            char buffer[512];
            glGetProgramInfoLog(_program, sizeof(buffer), nullptr, buffer);
            if (strlen(buffer) != 0) {
                std::cerr << "Error linking shaders: '" << buffer << "'" << std::endl;
                assert(false);
            }

            GLint success;
            glGetProgramiv(_program, GL_LINK_STATUS, &success);
            assert(success == GL_TRUE);
        }
#endif
    }

    void Shader::getUniformLocations() noexcept {
        for (const auto uniform: _uniforms) {
            uniform->location = glGetUniformLocation(_program, uniform->name.c_str());
#ifndef _NDEBUG
            if (uniform->location == -1)
                std::cerr << "Failed to get location for `" << uniform->name << "` uniform" << std::endl;
#endif
        }
    }
}