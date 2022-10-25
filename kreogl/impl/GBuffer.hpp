#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "RAII/Buffer.hpp"
#include "RAII/FrameBuffer.hpp"
#include "RAII/Texture.hpp"

namespace kreogl {
    class GBuffer {
    public:
        enum class Texture {
            Position,
            Normal,
            DiffuseAndShouldIgnoreLighting,
            Specular,
            UserData,
            Count
        };

    public:
        GBuffer(const glm::ivec2 & size) noexcept;

    public:
        void resize(const glm::ivec2 & size) noexcept;

    public:
        void bindForWriting() const noexcept;
        void bindForReading() const noexcept;

    public:
        struct MappedTexture {
            MappedTexture(GLuint pbo) noexcept;
            ~MappedTexture() noexcept;

            const float * getData() const noexcept { return _data; }

        private:
            GLuint _pbo;
            const float * _data;
        };

        MappedTexture getMappedTexture(Texture texture) const noexcept;

    public:
        const kreogl::Texture & getTexture(Texture texture) const noexcept { return _textures[(int)texture]; }
        GLuint getFrameBuffer() const noexcept { return _frameBuffer; }

	public:
		static constexpr auto TEXTURE_COMPONENTS = 4;

    private:
        FrameBuffer _frameBuffer;
        kreogl::Texture _depthTexture;
        glm::ivec2 _size;

        std::vector<kreogl::Texture> _textures;

        struct PBO {
            Buffer forWrite;
            Buffer forRead;
            bool init = false;
            mutable bool upToDate = false; // gets invalidated in bindForReading
        };
        mutable std::vector<PBO> _pbos;
    };
}