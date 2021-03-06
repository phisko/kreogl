#include "ShadowCubeGLSL.hpp"

namespace kreogl {
    const char * ShadowCubeGLSL::geom = R"(
#version 330

layout (triangles) in;
layout (triangle_strip, max_vertices=18) out;

uniform mat4 lightSpaceMatrices[6];
out vec4 FragPos;

void main() {
    for (int face = 0; face < 6; ++face) {
        gl_Layer = face;
        for (int i = 0; i < 3; ++i) {
            FragPos = gl_in[i].gl_Position;
            gl_Position = lightSpaceMatrices[face] * FragPos;
            EmitVertex();
        }
        EndPrimitive();
    }
}
)";

    const char * ShadowCubeGLSL::frag = R"(
#version 330

in vec4 FragPos;

uniform vec3 lightPos;
uniform float farPlane;

void main() {
    float lightDistance = length(FragPos.xyz - lightPos);
    lightDistance /= farPlane;
    gl_FragDepth = lightDistance;
}
)";
}