#include "point_light_glsl.hpp"

namespace kreogl {
	const char * point_light_glsl::frag = R"(
#version 330

uniform sampler2D gposition;
uniform sampler2D gnormal;
uniform sampler2D gdiffuse;
uniform sampler2D gspecular;

uniform vec3 view_pos;
uniform vec2 screen_size;

uniform vec4 color;
uniform vec3 position;

uniform float diffuse_strength;
uniform float specular_strength;

uniform float attenuation_constant;
uniform float attenuation_linear;
uniform float attenuation_quadratic;

out vec4 output_color;

float calc_shadow(vec3 world_pos, vec3 normal, vec3 light_dir);

vec3 calc_point_light(vec3 world_pos, vec3 normal, vec3 diffuse, vec3 specular) {
    vec3 view_dir = normalize(view_pos - world_pos);
    vec3 light_dir = normalize(position - world_pos);

    // diffuse shading
    float diff = max(dot(normal, light_dir), 0.0);
    diffuse = diffuse * diffuse_strength * diff;

    // specular shading
    vec3 reflect_dir = reflect(-light_dir, normal);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), 32.0);
    specular = specular * specular_strength * spec;

    // attenuation
    float dist = length(position - world_pos);
    float attenuation = 1.0 / (attenuation_constant + attenuation_linear * dist + attenuation_quadratic * (dist * dist));

    float shadow = calc_shadow(world_pos, normal, light_dir);
    return color.rgb * (1.0 - shadow) * (diffuse + specular) * attenuation;
}

void main() {
   	vec2 tex_coord = gl_FragCoord.xy / screen_size;

    vec4 diffuse_and_should_ignore_lighting = texture(gdiffuse, tex_coord);

    vec3 diffuse = diffuse_and_should_ignore_lighting.rgb;
    float should_ignore_lighting = diffuse_and_should_ignore_lighting.a;
    if (should_ignore_lighting == 1.0) {
        output_color = vec4(diffuse, 1.0);
        return;
    }

   	vec3 world_pos = texture(gposition, tex_coord).xyz;
   	vec3 normal = texture(gnormal, tex_coord).xyz;
    vec3 specular = texture(gspecular, tex_coord).rgb;

	output_color = vec4(calc_point_light(world_pos, normal, diffuse, specular), 1.0);
})";

	const char * point_light_glsl::get_direction::frag = R"(
#version 330

uniform vec3 position;

vec3 get_light_direction(vec3 world_pos) {
    return normalize(position - world_pos);
})";
}
