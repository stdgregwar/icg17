#version 330

uniform sampler2D leaves;
uniform float shine;

in vData {
    vec2 uv;
    vec3 color;
    vec3 normal;
} vertex;

layout (location = 0) out vec4 color;
layout (location = 1) out vec4 normal;

#include normal.glsl

void main() {
    normal.rgb = packNormal(vertex.normal);
    normal.a = 0;
    vec4 tex = texture(leaves,vertex.uv);
    if(tex.a < 0.8f) discard;
    color.rgb = tex.rgb*1.2;
    color.a = shine;
}
