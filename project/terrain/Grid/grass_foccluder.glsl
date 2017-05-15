#version 330

in vData {
    vec2 uv;
    vec3 color;
    float alpha;
    vec3 normal;
} vertex;

out vec3 color;

uniform sampler2D grasspatch;
uniform mat4 V;

#include sdoor.glsl

void main(void)
{
    vec4 tex = texture(grasspatch,vertex.uv);
    if(sdoor(gl_FragCoord.xy, vertex.alpha)) discard;
    if(tex.a < 0.8f) discard;
    color = vec3(0.2);
}

