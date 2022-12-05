#include "volumetric_lighting_glsl.hpp"

namespace kreogl {
	const char * volumetric_lighting_glsl::frag = R"(
#version 330

uniform sampler2D gposition;
uniform sampler2D gdiffuse;

uniform float scattering;
uniform int nb_steps;
uniform float default_step_length;
uniform float intensity;

uniform mat4 inverse_view;
uniform mat4 inverse_proj;
uniform vec3 view_pos;
uniform vec2 screen_size;

uniform vec4 color;

out vec4 out_color;

#define PI 3.1415926535897932384626433832795

bool is_position_lit(vec3 world_pos, float bias);
vec3 get_light_direction(vec3 world_pos);

float compute_scattering(float light_dot_view) {
    float result = 1.0 - scattering * scattering;
    result /= (4.0 * PI * pow(1.0 + scattering * scattering - (2.0 * scattering) * light_dot_view, 1.5));
    return result;
}

vec3 get_ray_to_pixel(vec2 screen_size) {
   	vec2 tex_coord = gl_FragCoord.xy / screen_size;
    vec2 normalized_screen_pos = tex_coord * 2.0 - vec2(1.0, 1.0);

	// raycast based on http://antongerdelan.net/opengl/raycasting.html
	vec4 ray_clip = vec4(normalized_screen_pos, -1.0, 1.0);
	vec4 ray_eye = inverse_proj * ray_clip;
	ray_eye = vec4(ray_eye.xy, -1.0, 0.0);
	vec3 ray_world = (inverse_view * ray_eye).xyz;
	return normalize(ray_world);
}

void main() {
   	vec2 tex_coord = gl_FragCoord.xy / screen_size;

	float should_ignore_lighting = texture(gdiffuse, tex_coord).a;
	if (should_ignore_lighting == 1) // don't apply lighting
		discard;

    vec4 world_pos = texture(gposition, tex_coord);
    vec3 light_dir = get_light_direction(world_pos.xyz);

    vec3 ray_dir;
    float step_length;
    if (world_pos.w == 0.0) { // "Empty" pixel, cast rays up to arbitrary distance
		ray_dir = get_ray_to_pixel(screen_size);
        step_length = default_step_length;
    }
    else { // "Full" pixel, cast rays up to it
        vec3 ray_vector = world_pos.xyz - view_pos;
        ray_dir = normalize(ray_vector);
        step_length = length(ray_vector) / nb_steps;
    }

    vec3 ray_step = ray_dir * step_length;
    vec3 current_pos = view_pos;

    const mat4 dither_pattern = mat4(
        vec4(0.0f, 0.5f, 0.125f, 0.625f),
        vec4(0.75f, 0.22f, 0.875f, 0.375f),
        vec4(0.1875f, 0.6875f, 0.0625f, 0.5625),
        vec4(0.9375f, 0.4375f, 0.8125f, 0.3125)
    );
    float dither_value = dither_pattern[int(gl_FragCoord.x) % 4][int(gl_FragCoord.y) % 4];
    current_pos += ray_step * dither_value;

    vec3 accum_fog = vec3(0.0);

    for (int i = 0; i < nb_steps; ++i) {
        const float bias = 0.0;
        if (is_position_lit(current_pos, bias))
            accum_fog += compute_scattering(dot(ray_dir, light_dir)) * color.rgb * intensity;
		current_pos += ray_step;
    }
    accum_fog /= nb_steps;

    out_color = vec4(accum_fog, 1.0);
})";
}