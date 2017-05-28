#version 330

uniform sampler2D trees_alb;

in vData {
    vec2 uv;
    vec3 normal;
} vertex;

layout (location = 0) out vec4 color;
layout (location = 1) out vec4 normal;

#include normal.glsl

void main() {
    normal.rgb = packNormal(vertex.normal);
    normal.a = 0;
    vec4 tex = texture(trees_alb,vertex.uv);
    if(length(tex.rgb) > 0.9) discard;
    color.rgb = tex.rgb*1.2;
    //color.rgb = vec3(1,0,0);
    color.a = 0;
}
