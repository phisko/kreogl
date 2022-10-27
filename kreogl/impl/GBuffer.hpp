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
		const glm::ivec2 & getSize() const noexcept { return _size; }

    public:
        void bindForWriting() const noexcept;
        void bindForReading() const noexcept;

    public:
		void getTextureContent(Texture texture, void * content) const noexcept;
		glm::vec4 readPixel(Texture texture, const glm::ivec2 & pixel) const noexcept;

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
    };
}