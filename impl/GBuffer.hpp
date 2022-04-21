#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "RAII/Buffer.hpp"
#include "RAII/FrameBuffer.hpp"
#include "RAII/Texture.hpp"

namespace kreogl {
    class GBuffer {
    public:
        static constexpr const char * attributes[] = {
            "position",
            "normal",
            "diffuse",
            "specular",
            "userdata"
        };
        static constexpr auto nbAttributes = sizeof(attributes) / sizeof(*attributes);

    public:
        GBuffer(const glm::ivec2 & size) noexcept;

    public:
        void resize(const glm::ivec2 & size) noexcept;

    public:
        void bindForWriting() noexcept;
        void bindForReading() noexcept;

    public:
        struct MappedTexture {
            MappedTexture(GLuint pbo) noexcept;
            ~MappedTexture() noexcept;

            const float * getData() const noexcept { return _data; }

        private:
            GLuint _pbo;
            const float * _data;
        };

        MappedTexture getTexture(size_t textureIndex) noexcept;

    public:
        size_t getTextureCount() const noexcept { return _textures.size(); }
        GLuint getFrameBuffer() const noexcept { return _frameBuffer; }

    private:
        FrameBuffer _frameBuffer;
        Texture _depthTexture;
        glm::ivec2 _size;

        std::vector<Texture> _textures;

        struct PBO {
            Buffer forWrite;
            Buffer forRead;
            bool init = false;
            bool upToDate = false;
        };
        std::vector<PBO> _pbos;
    };
}