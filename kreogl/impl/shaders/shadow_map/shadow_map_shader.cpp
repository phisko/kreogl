#include "shadow_map_shader.hpp"

// kreogl
#include "kreogl/camera.hpp"
#include "kreogl/impl/raii/scoped_bind_framebuffer.hpp"
#include "kreogl/impl/raii/scoped_gl_feature.hpp"
#include "kreogl/impl/kreogl_profiling.hpp"

namespace kreogl {
	template<typename T, typename Func>
	void shadow_map_shader::draw_impl(T & depth_map, Func && draw, const draw_params & params) noexcept {
		KREOGL_PROFILING_SCOPE;

		glViewport(0, 0, depth_map.get_size(), depth_map.get_size());
		glCullFace(GL_FRONT);

		const scoped_bind_framebuffer bound(depth_map.frame_buffer);
		const scoped_gl_feature depth(GL_DEPTH_TEST);

		use_without_uniform_check(); // Would need to pass the checker down to draw_to_texture

		draw();

		glCullFace(GL_BACK);

		const auto & viewport = params.camera.get_viewport();
		const auto & resolution = viewport.get_resolution();
		glViewport(0, 0, resolution.x, resolution.y);
	}

	void shadow_map_shader::draw(const directional_light & light, const draw_params & params) noexcept {
		KREOGL_PROFILING_SCOPE;

		light.cascaded_shadow_map.set_size(light.shadow_map_size);

		draw_impl(
			light.cascaded_shadow_map,
			[&]() noexcept {
				for (size_t i = 0; i < light.cascade_ends.size(); ++i) {
					const float cascade_start = (i == 0 ? KREOGL_SHADOW_MAP_NEAR_PLANE : light.cascade_ends[i - 1]);
					const float cascade_end = light.cascade_ends[i];
					if (cascade_start >= cascade_end)
						continue;
					const auto & texture = light.cascaded_shadow_map.textures[i];
					glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture, 0);
					draw_to_texture(texture, light.get_light_space_matrix_for_cascade(params, i), params);
				}
			},
			params
		);
	}

	void shadow_map_shader::draw(const spot_light & light, const draw_params & params) noexcept {
		KREOGL_PROFILING_SCOPE;

		light.shadow_map.set_size(light.shadow_map_size);

		draw_impl(
			light.shadow_map,
			[&]() noexcept {
				draw_to_texture(light.shadow_map.texture, light.get_light_space_matrix(), params);
			},
			params
		);
	}
}