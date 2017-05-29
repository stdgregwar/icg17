#version 330

uniform sampler2D trees_alb;
uniform sampler2D trees_nor;

in vData {
    vec2 uv;
    vec3 normal;
    mat3 tSpace;
} vertex;

layout (location = 0) out vec4 color;
layout (location = 1) out vec4 normal;

#include normal.glsl

void main() {
    vec3 texNormal = unpackNormal(trees_nor,vertex.uv);
    texNormal = vertex.tSpace * texNormal;
    normal.rgb = packNormal(texNormal);
    normal.a = 0;
    vec4 tex = texture(trees_alb,vertex.uv);
    if(distance(tex.rgb,vec3(1,1,1)) < 0.8f) discard;
    color.rgb = tex.rgb*0.9;
    //color.rgb = vec3(1,0,0);
    color.a = 0;
}
