#version 330

uniform sampler2D bark_alb;
uniform sampler2D bark_nor;

in vec3 vColor;
in vec3 vNormal;
in vec2 vUV;
in mat3 tSpace;

layout (location = 0) out vec4 color;
layout (location = 1) out vec4 normal;

#include normal.glsl

void main() {
    vec3 nmap = unpackNormal(bark_nor,vUV);
    nmap = tSpace*nmap;
    color.rgb = texture(bark_alb,vUV).rgb;
    color.a = 0;
    normal.a = 0;
    normal.rgb = packNormal(normalize(-vNormal+nmap));
}
