#include "TextShader.hpp"
#include "kreogl/impl/shaders/helpers/ApplyTransparency/ApplyTransparencyGLSL.hpp"

// kreogl
#include "kreogl/Camera.hpp"
#include "kreogl/World.hpp"
#include "FontLibrary.hpp"
#include "kreogl/impl/kreogl_profiling.hpp"

namespace kreogl {
	TextShader::TextShader() noexcept {
		KREOGL_PROFILING_SCOPE;

		init("TextShader");

		useWithoutUniformCheck();
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		_glsl.tex = (int)GBuffer::Texture::Count;

		glBindVertexArray(_vao);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	}

	void TextShader::addSourceFiles() noexcept {
		KREOGL_PROFILING_SCOPE;

		addSourceFile(TextGLSL::vert, GL_VERTEX_SHADER);
		addSourceFile(TextGLSL::frag, GL_FRAGMENT_SHADER);
		addSourceFile(ApplyTransparencyGLSL::frag, GL_FRAGMENT_SHADER);
	}

	std::vector<UniformBase *> TextShader::getUniforms() noexcept {
		KREOGL_PROFILING_SCOPE;

		return {
			&_glsl.model,
			&_glsl.view,
			&_glsl.proj,
			&_glsl.viewPos,
			&_glsl.tex,
			&_glsl.color,
			&_glsl.userData
		};
	}

	void TextShader::draw(const DrawParams & params) noexcept {
		KREOGL_PROFILING_SCOPE;

		auto uniformChecker = use(false);

		_glsl.viewPos = params.camera.getPosition();

		glActiveTexture((GLenum)(GL_TEXTURE0 + (int)GBuffer::Texture::Count));
		glBindVertexArray(_vao);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);

		_glsl.view = glm::mat4{ 1.f };
		_glsl.proj = glm::mat4{ 1.f };
		for (const auto text2D : params.world.getText2Ds()) {
			uniformChecker.shouldCheck = true;
			draw(params, *text2D);
		}

		_glsl.view = params.camera.getViewMatrix();
		_glsl.proj = params.camera.getProjMatrix();
		for (const auto text3D : params.world.getText3Ds()) {
			uniformChecker.shouldCheck = true;
			draw(params, *text3D);
		}
	}

	void TextShader::draw(const DrawParams & params, const Text & text) noexcept {
		KREOGL_PROFILING_SCOPE;

		auto & fontFamily = _fonts.getFontFamily(text.font);
		const auto font = fontFamily.getFont(text.fontSize);
		if (!font) {
			assert(false);
			return;
		}

		_glsl.model = text.transform;
		_glsl.color = text.color;
		_glsl.userData = text.userData;

		const auto scaleY = 1.f / (float)text.fontSize;
		const auto scaleX = scaleY / params.camera.getViewport().getAspectRatio();

		// Calculate text starting position
		const auto size = font->getSizeAndGenerateCharacters(text.text, scaleX, scaleY);
		const auto y = -size.y / 2.f;
		auto x = 0.f;
		switch (text.alignment) {
			case Text::Alignment::Left:
				break;
			case Text::Alignment::Center:
				x = -size.x / 2.f;
				break;
			case Text::Alignment::Right:
				x = -size.x;
				break;
			default:
				assert(false); // non-exhaustive switch
				break;
		}

		for (const auto c : text.text) {
			const auto character = font->getCharacter(c);
			if (!character) {
				assert(false);
				continue;
			}

			glBindTexture(GL_TEXTURE_2D, character->texture);

			const auto xpos = x + (float)character->bearing.x * scaleX;
			const auto ypos = y - (float)(character->size.y - character->bearing.y) * scaleY;

			const auto w = (float)character->size.x * scaleX;
			const auto h = (float)character->size.y * scaleY;

			const float vertices[6][4] = {
				{ xpos, ypos + h, 0.0, 0.0 },
				{ xpos, ypos, 0.0, 1.0 },
				{ xpos + w, ypos, 1.0, 1.0 },

				{ xpos, ypos + h, 0.0, 0.0 },
				{ xpos + w, ypos, 1.0, 1.0 },
				{ xpos + w, ypos + h, 1.0, 0.0 }
			};

			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			x += (float)(character->advance >> 6) * scaleX;
		}
	}
}