#include "viewport.hpp"

// stl
#include <cassert>

// kreogl
#include "kreogl/impl/raii/scoped_gl_feature.hpp"
#include "kreogl/impl/shaders/shader_pipeline.hpp"
#include "kreogl/impl/kreogl_profiling.hpp"

namespace kreogl {
	viewport::viewport(const construction_params & params) noexcept
	: _resolution(params.resolution),
	_on_screen_position(params.on_screen_position),
	_on_screen_size(params.on_screen_size),
	_z_order(params.z_order),
	_gbuffer(params.resolution)
	{
		KREOGL_PROFILING_SCOPE;

		glBindTexture(GL_TEXTURE_2D, _render_texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, _resolution.x, _resolution.y, 0, GL_RGBA, GL_FLOAT, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glBindFramebuffer(GL_FRAMEBUFFER, _frame_buffer);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _render_texture, 0);

		glBindTexture(GL_TEXTURE_2D, _depth_texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, _resolution.x, _resolution.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _depth_texture, 0);

		assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
	}

	void viewport::draw(const draw_params & params) const noexcept {
		KREOGL_PROFILING_SCOPE;

		fill_gbuffer(params);
		render_to_texture(params);
	}

	void viewport::fill_gbuffer(const draw_params & params) const noexcept {
		KREOGL_PROFILING_SCOPE;

		_gbuffer.bind_for_writing();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		const scoped_gl_feature depth(GL_DEPTH_TEST);
		params.shader_pipeline.run_shaders(shader_step::gbuffer, params);
	}

	void viewport::render_to_texture(const draw_params & params) const noexcept {
		KREOGL_PROFILING_SCOPE;

		_gbuffer.bind_for_reading();

		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _frame_buffer);
		glClear(GL_COLOR_BUFFER_BIT);
		glBlitFramebuffer(0, 0, _resolution.x, _resolution.y, 0, 0, _resolution.x, _resolution.y, GL_DEPTH_BUFFER_BIT, GL_NEAREST);

		params.shader_pipeline.run_shaders(shader_step::lighting, params);
		params.shader_pipeline.run_shaders(shader_step::post_lighting, params);
		params.shader_pipeline.run_shaders(shader_step::post_process, params);
	}

	void viewport::set_resolution(const glm::ivec2 & resolution) noexcept {
		KREOGL_PROFILING_SCOPE;

		_resolution = resolution;
		_gbuffer.resize(_resolution);

		glBindTexture(GL_TEXTURE_2D, _render_texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, _resolution.x, _resolution.y, 0, GL_RGBA, GL_FLOAT, nullptr);
	}
}