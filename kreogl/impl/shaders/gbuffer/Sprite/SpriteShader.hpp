#pragma once

#include "kreogl/impl/shaders/Shader.hpp"
#include "kreogl/impl/shaders/Singleton.hpp"

#include "SpriteGLSL.hpp"

#include "kreogl/Sprite.hpp"

namespace kreogl {
	class SpriteShader : public Shader, public Singleton<SpriteShader> {
	public:
		SpriteShader() noexcept;

	private:
		void draw(const DrawParams & params) noexcept override;
		void addSourceFiles() noexcept override;
		std::vector<UniformBase *> getUniforms() noexcept override;

	private:
		void draw(const DrawParams & params, const Sprite & sprite) noexcept;

	private:
		SpriteGLSL _glsl;
	};
}