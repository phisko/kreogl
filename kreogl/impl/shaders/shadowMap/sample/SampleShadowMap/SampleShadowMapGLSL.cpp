#include "SampleShadowMapGLSL.hpp"

namespace kreogl {
    const char * SampleShadowMapGLSL::frag = R"(
#version 330

uniform sampler2D shadowMap;
uniform mat4 lightSpaceMatrix;
uniform float minBias;
uniform float maxBias;
uniform int pcfSamples;

vec2 getShadowMapValue(vec3 worldPos) {
    vec4 worldPosLightSpace = lightSpaceMatrix * vec4(worldPos, 1.0);
    vec3 projCoords = worldPosLightSpace.xyz / worldPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5; // transform to [0,1] range

    float objectDepth = projCoords.z;
    float shadowMapValue = texture(shadowMap, projCoords.xy).r;
    return vec2(shadowMapValue, objectDepth);
}

bool isPositionLit(vec3 worldPos, float bias) {
    vec2 shadow = getShadowMapValue(worldPos);
    float shadowMapValue = shadow.x;
    float objectDepth = shadow.y;
    return shadowMapValue > objectDepth - bias;
}

float calcShadow(vec3 worldPos, vec3 normal, vec3 lightDir) {
    vec4 worldPosLightSpace = lightSpaceMatrix * vec4(worldPos, 1.0);
    vec3 projCoords = worldPosLightSpace.xyz / worldPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5; // transform to [0,1] range

    float currentDepth = projCoords.z;
    float closestDepth = texture(shadowMap, projCoords.xy).r;

    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);

    // calculate bias (based on depth map resolution and slope)
    float bias = max(maxBias * (1.0 - dot(normal, lightDir)), minBias);

    for (int x = -pcfSamples; x <= pcfSamples; ++x) {
        for (int y = -pcfSamples; y <= pcfSamples; ++y) {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= (pcfSamples * 2 + 1) * (pcfSamples * 2 + 1);

    if (projCoords.z > 1.0)
        shadow = 0.0;

    return shadow;
})";
}