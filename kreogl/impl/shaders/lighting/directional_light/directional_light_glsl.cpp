#include "directional_light_glsl.hpp"

namespace kreogl {
	const char * directional_light_glsl::frag = R"(
#version 330

uniform sampler2D gposition;
uniform sampler2D gnormal;
uniform sampler2D gdiffuse;
uniform sampler2D gspecular;

uniform vec3 view_pos;
uniform vec2 screen_size;

uniform vec4 color;
uniform vec3 direction;

uniform float ambient_strength;
uniform float diffuse_strength;
uniform float specular_strength;

out vec4 output_color;

const bool debug_csm = false;
int get_cascade_index(vec3 world_pos); // for debug only

float calc_shadow(vec3 world_pos, vec3 normal, vec3 light_dir);

vec3 calc_dir_light(vec3 world_pos, vec3 normal, vec3 diffuse, vec3 specular) {
    vec3 view_dir = normalize(view_pos - world_pos);
    vec3 light_dir = normalize(-direction);

	// ambient shading
	vec3 ambient = diffuse * ambient_strength;

    // diffuse shading
    float diff = max(dot(normal, light_dir), 0.0);
    diffuse = diffuse * diffuse_strength * diff;

    // specular shading
    vec3 reflect_dir = reflect(-light_dir, normal);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), 32.0);
    specular = specular * specular_strength * spec;

    // combine results
    float shadow = calc_shadow(world_pos, normal, light_dir);
    return color.rgb * (ambient + (1.0 - shadow) * (diffuse + specular));
}

void main() {
   	vec2 tex_coord = gl_FragCoord.xy / screen_size;

   	vec3 world_pos = texture(gposition, tex_coord).xyz;
   	vec3 normal = texture(gnormal, tex_coord).xyz;

	vec4 diffuse_and_should_ignore_lighting = texture(gdiffuse, tex_coord);
	vec3 diffuse = diffuse_and_should_ignore_lighting.rgb;
	vec3 specular = texture(gspecular, tex_coord).rgb;

	float ignore_lighting = diffuse_and_should_ignore_lighting.a;

	output_color = vec4(diffuse, 1.0);
	if (ignore_lighting == 0.0)
		output_color = vec4(calc_dir_light(world_pos, normal, diffuse, specular), 1.0);

	if (debug_csm) {
		int index = get_cascade_index(world_pos);
		if (index == 0)
			output_color = output_color * vec4(1.0, 0.0, 0.0, 1.0);
		else if (index == 1)
			output_color = output_color * vec4(0.0, 1.0, 0.0, 1.0);
		else if (index == 2)
			output_color = output_color * vec4(0.0, 0.0, 1.0, 1.0);
		else if (index == 3)
			output_color = output_color * vec4(1.0, 1.0, 0.0, 1.0);
		else if (index == 4)
			output_color = output_color * vec4(1.0, 0.0, 1.0, 1.0);
		else if (index == 5)
			output_color = output_color * vec4(0.0, 1.0, 1.0, 1.0);
		else if (index == 6)
			output_color = output_color * vec4(1.0, 0.5, 0.5, 1.0);
		else if (index == 7)
			output_color = output_color * vec4(0.5, 1.0, 0.5, 1.0);
	}
})";

	const char * directional_light_glsl::get_direction::frag = R"(
#version 330

uniform vec3 direction;

vec3 get_light_direction(vec3 world_pos) {
    return -direction;
})";
}