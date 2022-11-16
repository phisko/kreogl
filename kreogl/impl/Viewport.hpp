#pragma once

#include <glm/glm.hpp>
#include "DrawParams.hpp"
#include "GBuffer.hpp"
#include "RAII/FrameBuffer.hpp"
#include "RAII/Texture.hpp"

namespace kreogl {
	class Camera;

	class KREOGL_EXPORT Viewport {
	public:
		struct KREOGL_EXPORT ConstructionParams {
			glm::ivec2 resolution = { 0, 0 };
			glm::vec2 onScreenPosition = { 0, 0 };
			glm::vec2 onScreenSize = { 1, 1 };
			float zOrder = 1.f;
		};
		Viewport(const ConstructionParams & params) noexcept;

		void draw(const DrawParams & params) const noexcept;

	public:
		const glm::ivec2 & getResolution() const noexcept { return _resolution; }
		void setResolution(const glm::ivec2 & resolution) noexcept;

		float getAspectRatio() const noexcept { return (float)_resolution.x / (float)_resolution.y; }

		const glm::vec2 & getOnScreenPosition() const noexcept { return _onScreenPosition; }
		void setOnScreenPosition(const glm::vec2 & onScreenPosition) noexcept { _onScreenPosition = onScreenPosition; }

		const glm::vec2 & getOnScreenSize() const noexcept { return _onScreenSize; }
		void setOnScreenSize(const glm::vec2 & onScreenSize) noexcept { _onScreenSize = onScreenSize; }

		float getZOrder() const noexcept { return _zOrder; }
		void setZOrder(float zOrder) noexcept { _zOrder = zOrder; }

		GLuint getFrameBuffer() const noexcept { return _frameBuffer; }
		GLuint getRenderTexture() const noexcept { return _renderTexture; }

		GBuffer & getGBuffer() noexcept { return _gbuffer; }
		const GBuffer & getGBuffer() const noexcept { return _gbuffer; }

	private:
		void fillGBuffer(const DrawParams & params) const noexcept;
		void renderToTexture(const DrawParams & params) const noexcept;

	private:
		glm::ivec2 _resolution;
		glm::vec2 _onScreenPosition;
		glm::vec2 _onScreenSize;
		float _zOrder;

		FrameBuffer _frameBuffer;
		Texture _renderTexture;
		Texture _depthTexture;
		GBuffer _gbuffer;
	};
}