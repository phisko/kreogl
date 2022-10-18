#include "SkyboxShader.hpp"

#include "kreogl/Camera.hpp"
#include "kreogl/World.hpp"
#include "kreogl/impl/RAII/ScopedGLFeature.hpp"
#include "kreogl/impl/kreogl_profiling.hpp"

namespace kreogl {
    SkyboxShader::SkyboxShader() noexcept {
        KREOGL_PROFILING_SCOPE;

        init("SkyboxShader");

        useWithoutUniformCheck();
        _glsl.tex = (int)GBuffer::Texture::Count;
    }

    void SkyboxShader::addSourceFiles() noexcept {
        KREOGL_PROFILING_SCOPE;

        addSourceFile(SkyboxGLSL::vert, GL_VERTEX_SHADER);
        addSourceFile(SkyboxGLSL::frag, GL_FRAGMENT_SHADER);
    }

    std::vector<UniformBase *> SkyboxShader::getUniforms() noexcept {
        KREOGL_PROFILING_SCOPE;

        return {
            &_glsl.proj,
            &_glsl.view,
            &_glsl.tex,
            &_glsl.color
        };
    }

    void SkyboxShader::draw(const DrawParams & params) noexcept {
        KREOGL_PROFILING_SCOPE;

        if (!params.world.skybox.texture.texture)
            return;

        const auto uniformChecker = use();

        GLint fbo;
        glGetIntegerv(GL_FRAMEBUFFER_BINDING, &fbo);
        assert(fbo != 0);

        const ScopedGLFeature blend(GL_BLEND);
        glBlendEquation(GL_FUNC_ADD);
        glBlendFunc(GL_ONE, GL_ONE);

        const ScopedGLFeature depth(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        _glsl.view = glm::mat4(glm::mat3(params.camera.getViewMatrix()));
        _glsl.proj = params.camera.getProjMatrix();

        glActiveTexture((GLenum)(GL_TEXTURE0 + (int)GBuffer::Texture::Count));

        _glsl.color = params.world.skybox.color;

        glBindTexture(GL_TEXTURE_CUBE_MAP, *params.world.skybox.texture.texture);
        drawSkyBox();

        glDepthFunc(GL_LESS);
    }

    void SkyboxShader::drawSkyBox() noexcept {
        KREOGL_PROFILING_SCOPE;

        static GLuint vao = 0;
        static GLuint vbo;
        if (vao == 0) {
            const float vertices[] = {
                // positions
                -1.0f,  1.0f, -1.0f,
                -1.0f, -1.0f, -1.0f,
                1.0f, -1.0f, -1.0f,
                1.0f, -1.0f, -1.0f,
                1.0f,  1.0f, -1.0f,
                -1.0f,  1.0f, -1.0f,

                -1.0f, -1.0f,  1.0f,
                -1.0f, -1.0f, -1.0f,
                -1.0f,  1.0f, -1.0f,
                -1.0f,  1.0f, -1.0f,
                -1.0f,  1.0f,  1.0f,
                -1.0f, -1.0f,  1.0f,

                1.0f, -1.0f, -1.0f,
                1.0f, -1.0f,  1.0f,
                1.0f,  1.0f,  1.0f,
                1.0f,  1.0f,  1.0f,
                1.0f,  1.0f, -1.0f,
                1.0f, -1.0f, -1.0f,

                -1.0f, -1.0f,  1.0f,
                -1.0f,  1.0f,  1.0f,
                1.0f,  1.0f,  1.0f,
                1.0f,  1.0f,  1.0f,
                1.0f, -1.0f,  1.0f,
                -1.0f, -1.0f,  1.0f,

                -1.0f,  1.0f, -1.0f,
                1.0f,  1.0f, -1.0f,
                1.0f,  1.0f,  1.0f,
                1.0f,  1.0f,  1.0f,
                -1.0f,  1.0f,  1.0f,
                -1.0f,  1.0f, -1.0f,

                -1.0f, -1.0f, -1.0f,
                -1.0f, -1.0f,  1.0f,
                1.0f, -1.0f, -1.0f,
                1.0f, -1.0f, -1.0f,
                -1.0f, -1.0f,  1.0f,
                1.0f, -1.0f,  1.0f
            };

            glGenVertexArrays(1, &vao);
            glGenBuffers(1, &vbo);
            glBindVertexArray(vao);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        }

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}