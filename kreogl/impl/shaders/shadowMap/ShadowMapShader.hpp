#pragma once

#include "kreogl/impl/shaders/Shader.hpp"
#include "kreogl/lights/DirectionalLight.hpp"
#include "kreogl/lights/SpotLight.hpp"
#include "kreogl/impl/DrawParams.hpp"

namespace kreogl {
	class KREOGL_EXPORT ShadowMapShader : public Shader {
	protected:
		virtual void drawToTexture(GLuint texture, const glm::mat4 & lightSpaceMatrix, const DrawParams & params) noexcept = 0;

	public:
		void draw(const DirectionalLight & light, const DrawParams & params) noexcept;
		void draw(const SpotLight & light, const DrawParams & params) noexcept;

	private:
		void draw(const DrawParams & params) noexcept final {}

		template<typename T, typename Func>
		void drawImpl(T & shadowMap, Func && draw, const DrawParams & params) noexcept;
	};
}