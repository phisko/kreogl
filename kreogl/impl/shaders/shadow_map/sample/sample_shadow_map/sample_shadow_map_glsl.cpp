#include "sample_shadow_map_glsl.hpp"

namespace kreogl {
	const char * sample_shadow_map_glsl::frag = R"(
#version 330

uniform sampler2D shadow_map;
uniform mat4 light_space_matrix;
uniform float min_bias;
uniform float max_bias;
uniform int pcf_samples;

vec2 get_shadow_map_value(vec3 world_pos) {
    vec4 world_pos_light_space = light_space_matrix * vec4(world_pos, 1.0);
    vec3 proj_coords = world_pos_light_space.xyz / world_pos_light_space.w;
    proj_coords = proj_coords * 0.5 + 0.5; // transform to [0,1] range

    float object_depth = proj_coords.z;
    float shadow_map_value = texture(shadow_map, proj_coords.xy).r;
    return vec2(shadow_map_value, object_depth);
}

bool is_position_lit(vec3 world_pos, float bias) {
    vec2 shadow = get_shadow_map_value(world_pos);
    float shadow_map_value = shadow.x;
    float object_depth = shadow.y;
    return shadow_map_value > object_depth - bias;
}

float calc_shadow(vec3 world_pos, vec3 normal, vec3 light_dir) {
    vec4 world_pos_light_space = light_space_matrix * vec4(world_pos, 1.0);
    vec3 proj_coords = world_pos_light_space.xyz / world_pos_light_space.w;
    proj_coords = proj_coords * 0.5 + 0.5; // transform to [0,1] range

    float current_depth = proj_coords.z;
    float closest_depth = texture(shadow_map, proj_coords.xy).r;

    float shadow = 0.0;
    vec2 texel_size = 1.0 / textureSize(shadow_map, 0);

    // calculate bias (based on depth map resolution and slope)
    float bias = max(max_bias * (1.0 - dot(normal, light_dir)), min_bias);

    for (int x = -pcf_samples; x <= pcf_samples; ++x) {
        for (int y = -pcf_samples; y <= pcf_samples; ++y) {
            float pcf_depth = texture(shadow_map, proj_coords.xy + vec2(x, y) * texel_size).r;
            shadow += current_depth - bias > pcf_depth ? 1.0 : 0.0;
        }
    }
    shadow /= (pcf_samples * 2 + 1) * (pcf_samples * 2 + 1);

    if (proj_coords.z > 1.0)
        shadow = 0.0;

    return shadow;
})";
}