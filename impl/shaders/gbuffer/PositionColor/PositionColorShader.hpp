#pragma once

#include "PositionColorGLSL.hpp"
#include "impl/shaders/Shader.hpp"
#include "impl/shaders/Singleton.hpp"

namespace kreogl {
    class PositionColorShader : public Shader, public Singleton<PositionColorShader> {
	public:
        PositionColorShader() noexcept;
		void draw(const DrawParams & params) noexcept override;

    private:
        void addSourceFiles() noexcept override;
        std::vector<UniformBase *> getUniforms() noexcept override;

    private:
        PositionColorGLSL _glsl;
    };
}
