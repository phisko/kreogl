#include "shader_pipeline.hpp"

// stl
#include <algorithm>

// kreogl
#include "kreogl/impl/kreogl_profiling.hpp"

// shaders
#ifdef KREOGL_DEFAULT_SHADERS
// lighting
#include "kreogl/impl/shaders/lighting/directional_light/directional_light_shader.hpp"
#include "kreogl/impl/shaders/lighting/point_light/point_light_shader.hpp"
#include "kreogl/impl/shaders/lighting/spot_light/spot_light_shader.hpp"

// volumetric lighting
#include "kreogl/impl/shaders/post_lighting/volumetric_lighting/volumetric_directional_light/volumetric_directional_light_shader.hpp"
#include "kreogl/impl/shaders/post_lighting/volumetric_lighting/volumetric_point_light/volumetric_point_light_shader.hpp"
#include "kreogl/impl/shaders/post_lighting/volumetric_lighting/volumetric_spot_light/volumetric_spot_light_shader.hpp"

// voxels
#include "kreogl/impl/shaders/gbuffer/position_color/position_color_shader.hpp"
#include "kreogl/impl/shaders/shadow_map/position_shadow_cube/position_shadow_cube_shader.hpp"
#include "kreogl/impl/shaders/shadow_map/position_shadow_map/position_shadow_map_shader.hpp"

// skeletal
#include "kreogl/impl/shaders/gbuffer/skeletal_textured/skeletal_textured_shader.hpp"
#include "kreogl/impl/shaders/shadow_map/skeletal_shadow_map/skeletal_shadow_map_shader.hpp"
#include "kreogl/impl/shaders/shadow_map/skeletal_shadow_cube/skeletal_shadow_cube_shader.hpp"

// misc
#include "kreogl/impl/shaders/gbuffer/debug/debug_shader.hpp"
#include "kreogl/impl/shaders/gbuffer/sprite/sprite_shader.hpp"
#include "kreogl/impl/shaders/gbuffer/text/text_shader.hpp"
#include "kreogl/impl/shaders/post_process/light_sphere/light_sphere_shader.hpp"
#include "kreogl/impl/shaders/post_process/skybox/skybox_shader.hpp"
#endif

namespace kreogl {
#ifdef KREOGL_DEFAULT_SHADERS
	const shader_pipeline & shader_pipeline::get_default_shaders() noexcept {
		static const shader_pipeline default_shaders = []() noexcept {
			KREOGL_PROFILING_SCOPE;

			shader_pipeline pipeline;

			// lighting
			pipeline.add_shader(shader_step::lighting, directional_light_shader::get_singleton());
			pipeline.add_shader(shader_step::lighting, point_light_shader::get_singleton());
			pipeline.add_shader(shader_step::lighting, spot_light_shader::get_singleton());

			// Volumetric
			pipeline.add_shader(shader_step::post_lighting, volumetric_directional_light_shader::get_singleton());
			pipeline.add_shader(shader_step::post_lighting, volumetric_point_light_shader::get_singleton());
			pipeline.add_shader(shader_step::post_lighting, volumetric_spot_light_shader::get_singleton());

			// Voxels
			pipeline.add_shader(shader_step::gbuffer, position_color_shader::get_singleton());
			pipeline.add_shader(shader_step::shadow_map, position_shadow_map_shader::get_singleton());
			pipeline.add_shader(shader_step::shadow_cube, position_shadow_cube_shader::get_singleton());

			// Skeletal meshes
			pipeline.add_shader(shader_step::gbuffer, skeletal_textured_shader::get_singleton());
			pipeline.add_shader(shader_step::shadow_map, skeletal_shadow_map_shader::get_singleton());
			pipeline.add_shader(shader_step::shadow_cube, skeletal_shadow_cube_shader::get_singleton());

			// Misc
			pipeline.add_shader(shader_step::gbuffer, debug_shader::get_singleton());
			pipeline.add_shader(shader_step::gbuffer, sprite_shader::get_singleton());
			pipeline.add_shader(shader_step::gbuffer, text_shader::get_singleton());
			pipeline.add_shader(shader_step::post_process, light_sphere_shader::get_singleton());
			pipeline.add_shader(shader_step::post_process, skybox_shader::get_singleton());

			return pipeline;
		}();

		return default_shaders;
	}
#endif

	void shader_pipeline::add_shader(shader_step step, shader & shader) noexcept {
		KREOGL_PROFILING_SCOPE;

		_shaders_per_step[step].push_back(&shader);
	}

	void shader_pipeline::remove_shader(shader_step step, const shader & shader) noexcept {
		KREOGL_PROFILING_SCOPE;

		const auto shaders = _shaders_per_step.find(step);
		if (shaders == _shaders_per_step.end())
			return;
		const auto it = std::ranges::find(shaders->second, &shader);
		if (it == shaders->second.end())
			return;
		shaders->second.erase(it);
	}

	void shader_pipeline::run_shaders(shader_step step, const draw_params & params) const noexcept {
		KREOGL_PROFILING_SCOPE;

		const auto it = _shaders_per_step.find(step);
		if (it == _shaders_per_step.end())
			return;

		const auto & shaders = it->second;
		for (const auto shader : shaders)
			shader->draw(params);
	}

	const std::vector<shader *> * shader_pipeline::get_shaders(shader_step step) const noexcept {
		KREOGL_PROFILING_SCOPE;

		const auto it = _shaders_per_step.find(step);
		if (it == _shaders_per_step.end())
			return nullptr;
		return &it->second;
	}
}