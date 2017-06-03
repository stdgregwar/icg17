#version 330

in vData {
    vec2 uv;
    vec3 color;
    float alpha;
    vec3 normal;
} vertex;

layout (location = 0) out vec4 color;
layout (location = 1) out vec4 normal;

uniform sampler2D grasspatch;
uniform sampler2DShadow shadowmap;
uniform mat4 V;
uniform mat4 MV;
uniform mat4 iV;
uniform vec3 l_color;
uniform mat4 l_iV;

#include sdoor.glsl
#include normal.glsl

void main(void)
{
    normal.xyz = packNormal(vertex.normal);
    vec4 tex = texture(grasspatch,vertex.uv);
    tex.rgb *= 1.5;
    if(sdoor(gl_FragCoord.xy, vertex.alpha)) discard;
    if(tex.a < 0.8f) discard;
    color.rgb = vertex.color*tex.rgb;
    color.a = 0.2;
    normal.a = 0.2;
}
