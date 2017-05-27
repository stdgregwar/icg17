#version 330

in vec3 vColor;
in vec3 vNormal;
in vec2 vUV;

layout (location = 0) out vec4 color;
layout (location = 1) out vec4 normal;

void main() {
    color.rgb = vColor + vec3(vUV,0);
    normal.rgb = vNormal;
}
