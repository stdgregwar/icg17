#version 330

in vData {
    vec2 uv;
    vec3 color;
    float alpha;
    vec3 normal;
    vec4 shadow_pos;
} vertex;

out vec3 color;

uniform sampler2D grasspatch;
uniform sampler2DShadow shadowmap;
uniform mat4 V;
uniform mat4 iV;
uniform vec3 l_color;
uniform mat4 l_iV;

#include sdoor.glsl
#include rand.glsl
#include clipspace.glsl
#include shadows.glsl

void main(void)
{
    vec3 light = (l_iV * vec4(0,0,1,0)).xyz;

    light = (V*vec4(light,0)).xyz;

    float diff = clamp(dot(vertex.normal,light),0,1);
    vec4 tex = texture(grasspatch,vertex.uv);
    tex.rgb *= 1.5;
    if(sdoor(gl_FragCoord.xy, vertex.alpha)) discard;
    if(tex.a < 0.8f) discard;
    diff = diffFromShadows(shadowmap,vertex.shadow_pos,diff,vertex.normal,light)+0.2;
    color = vertex.color*tex.rgb*l_color*diff;
    //color = vec3(diff);
}
