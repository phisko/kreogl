#pragma once

#include "PositionEmptyNormalColorGLSL.hpp"
#include "impl/shaders/Shader.hpp"
#include "impl/shaders/Singleton.hpp"

namespace kreogl {
    class PositionEmptyNormalColorShader : public Shader, public Singleton<PositionEmptyNormalColorShader> {
	public:
        PositionEmptyNormalColorShader() noexcept;
		void draw(const DrawParams & params) noexcept override;

    private:
        void addSourceFiles() noexcept override;
        std::vector<UniformBase *> getUniforms() noexcept override;

    private:
        PositionEmptyNormalColorGLSL _glsl;
    };
}