#include "sample_cascaded_shadow_map_glsl.hpp"

#define MACRO_AS_STRING_IMPL(macro) #macro
#define MACRO_AS_STRING(macro) MACRO_AS_STRING_IMPL(macro)

namespace kreogl {
	const char * sample_cascaded_shadow_map_glsl::frag = R"(
#version 450

const int CSM_COUNT = )" MACRO_AS_STRING(KREOGL_MAX_CSM_COUNT) R"(;
uniform sampler2D shadow_map[CSM_COUNT];
uniform mat4 light_space_matrix[CSM_COUNT];
uniform float cascade_end[CSM_COUNT];
uniform int cascade_count;
uniform float min_bias;
uniform float max_bias;
uniform int pcf_samples;

uniform mat4 view;

int get_cascade_index(vec3 world_pos) {
	float clip_space_pos_z = abs((view * vec4(world_pos, 1.0)).z);
	for (int i = 0; i < cascade_count; ++i)
		if (clip_space_pos_z <= cascade_end[i])
			return i;
	return -1;
}

vec2 get_shadow_map_value(vec3 world_pos) {
	int index = get_cascade_index(world_pos);
	if (index < 0)
		return vec2(-1, -1);

    vec4 world_pos_light_space = light_space_matrix[index] * vec4(world_pos, 1.0);
    vec3 proj_coords = world_pos_light_space.xyz / world_pos_light_space.w;
    proj_coords = proj_coords * 0.5 + 0.5; // transform to [0,1] range

    float object_depth = proj_coords.z;
    float shadow_map_value = texture(shadow_map[index], proj_coords.xy).r;
    return vec2(shadow_map_value, object_depth);
}

bool is_position_lit(vec3 world_pos, float bias) {
    vec2 shadow = get_shadow_map_value(world_pos);

    float shadow_map_value = shadow.x;
    float object_depth = shadow.y;

    // not covered by cascades
    if (shadow_map_value < 0.0)
        return false;

    // corrupted light space matrix for index :(
    if (object_depth > 1.0)
        return false;

    return shadow_map_value > object_depth - bias;
}

float calc_shadow_with_csm(int index, vec3 world_pos, vec3 normal, vec3 light_dir) {
    vec4 world_pos_light_space = light_space_matrix[index] * vec4(world_pos, 1.0);
    vec3 proj_coords = world_pos_light_space.xyz / world_pos_light_space.w;
    proj_coords = proj_coords * 0.5 + 0.5; // transform to [0,1] range

    float world_pos_depth = proj_coords.z;
    if (world_pos_depth > 1.0)
        return 0.0;

    // calculate bias (based on depth map resolution and slope)
    float bias = max(max_bias * (1.0 - dot(normal, light_dir)), min_bias);
    bias *= 1 / (cascade_end[index] * 0.5f);

    float shadow = 0.0;
    vec2 texel_size = 1.0 / textureSize(shadow_map[index], 0);

    for (int x = -pcf_samples; x <= pcf_samples; ++x) {
        for (int y = -pcf_samples; y <= pcf_samples; ++y) {
            float pcf_depth = texture(shadow_map[index], proj_coords.xy + vec2(x, y) * texel_size).r;
            shadow += world_pos_depth - bias > pcf_depth ? 1.0 : 0.0;
        }
    }
    shadow /= (pcf_samples * 2 + 1) * (pcf_samples * 2 + 1);

    return shadow;
}

float calc_shadow(vec3 world_pos, vec3 normal, vec3 light_dir) {
	int index = get_cascade_index(world_pos);
	if (index >= 0)
		return calc_shadow_with_csm(index, world_pos, normal, light_dir);
	return 0.0;
})";
}