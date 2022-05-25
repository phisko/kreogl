#include "SampleCascadedShadowMapGLSL.hpp"

#define MACRO_AS_STRING_IMPL(macro) #macro
#define MACRO_AS_STRING(macro) MACRO_AS_STRING_IMPL(macro)

namespace kreogl {
    const char * SampleCascadedShadowMapGLSL::frag = R"(
#version 450

const int CSM_COUNT = )" MACRO_AS_STRING(KREOGL_MAX_CSM_COUNT) R"(;
uniform sampler2D shadowMap[CSM_COUNT];
uniform mat4 lightSpaceMatrix[CSM_COUNT];
uniform float cascadeEnd[CSM_COUNT];
uniform int cascadeCount;
uniform float minBias;
uniform float maxBias;
uniform int pcfSamples;

uniform mat4 view;

int getCascadeIndex(vec3 worldPos) {
	float clipSpacePosZ = abs((view * vec4(worldPos, 1.0)).z);
	for (int i = 0; i < cascadeCount; ++i)
		if (clipSpacePosZ <= cascadeEnd[i])
			return i;
	return -1;
}

vec2 getShadowMapValue(vec3 worldPos) {
	int index = getCascadeIndex(worldPos);
	if (index < 0)
		return vec2(1, 1);

    vec4 worldPosLightSpace = lightSpaceMatrix[index] * vec4(worldPos, 1.0);
    vec3 projCoords = worldPosLightSpace.xyz / worldPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5; // transform to [0,1] range

    float objectDepth = projCoords.z;
    float shadowMapValue = texture(shadowMap[index], projCoords.xy).r;
    return vec2(shadowMapValue, objectDepth);
}

float calcShadowWithCSM(int index, vec3 worldPos, vec3 normal, vec3 lightDir) {
    vec4 worldPosLightSpace = lightSpaceMatrix[index] * vec4(worldPos, 1.0);
    vec3 projCoords = worldPosLightSpace.xyz / worldPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5; // transform to [0,1] range

    float worldPosDepth = projCoords.z;
    if (worldPosDepth > 1.0)
        return 0.0;

    // calculate bias (based on depth map resolution and slope)
    float bias = max(maxBias * (1.0 - dot(normal, lightDir)), minBias);
    bias *= 1 / (cascadeEnd[index] * 0.5f);

    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap[index], 0);

    for (int x = -pcfSamples; x <= pcfSamples; ++x) {
        for (int y = -pcfSamples; y <= pcfSamples; ++y) {
            float pcfDepth = texture(shadowMap[index], projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += worldPosDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= (pcfSamples * 2 + 1) * (pcfSamples * 2 + 1);

    return shadow;
}

float calcShadow(vec3 worldPos, vec3 normal, vec3 lightDir) {
	int index = getCascadeIndex(worldPos);
	if (index >= 0)
		return calcShadowWithCSM(index, worldPos, normal, lightDir);
	return 0.0;
})";
}