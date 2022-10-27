#pragma once

#include "kreogl/impl/RAII/FrameBuffer.hpp"
#include "kreogl/impl/RAII/Texture.hpp"

namespace kreogl {
	struct ShadowMap {
		ShadowMap() noexcept;

		FrameBuffer frameBuffer;
		Texture texture;

		void setSize(int size) noexcept;
		int getSize() const noexcept { return _size; }

	private:
		int _size = -1;
	};
}