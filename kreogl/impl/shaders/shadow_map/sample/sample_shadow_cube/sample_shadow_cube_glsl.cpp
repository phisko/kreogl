#include "sample_shadow_cube_glsl.hpp"

namespace kreogl {
	const char * sample_shadow_cube_glsl::frag = R"(
#version 330

uniform samplerCube shadow_map;
uniform vec3 position;
uniform vec3 view_pos;
uniform float far_plane;
uniform float min_bias;
uniform float max_bias;

vec2 get_shadow_map_value(vec3 world_pos) {
    vec3 light_dir = world_pos - position;
    float shadow_map_value = texture(shadow_map, light_dir).r * far_plane;
    float object_depth = length(light_dir);
    return vec2(shadow_map_value, object_depth);
}

bool is_position_lit(vec3 world_pos, float bias) {
    vec2 shadow = get_shadow_map_value(world_pos);
    float shadow_map_value = shadow.x;
    float object_depth = shadow.y;
    return shadow_map_value > object_depth - bias;
}

float calc_shadow(vec3 world_pos, vec3 normal, vec3 lightDirIgnore) {
	vec3 light_dir = world_pos - position;
	float current_depth = length(light_dir);

	vec3 sample_offset_directions[20] = vec3[](
	   vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1),
	   vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
	   vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
	   vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
	   vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
	);
	const int samples = 20;

	float view_distance = length(view_pos - world_pos);
	float sample_radius = (1.0 + (view_distance / far_plane)) / 25.0;

    // calculate bias (based on depth map resolution and slope)
    float bias = max(max_bias * (1.0 - dot(normal, light_dir)), min_bias);

	float shadow = 0.0;
	for (int i = 0; i < samples; ++i) {
		float closest_depth = texture(shadow_map, light_dir + sample_offset_directions[i] * sample_radius).r;
		closest_depth *= far_plane;
		if (current_depth - bias > closest_depth)
			shadow += 1.0;
	}
	shadow /= float(samples);

	return shadow;
})";
}