#version 330

uniform mat4 iV;

in vec3 position;
in vec4 dir;

out vData {
    int tex;
    vec3 pos;
    vec3 dir;
    vec3 view_dir;
} vertex;

#include rand.glsl

void main() {
    vertex.pos = position;
    vertex.dir = dir.xyz;
    vec4 eye = iV * vec4(0,0,-1,0);
    vertex.view_dir = normalize(eye.xyz);
    vertex.tex =  3+int(round((dir.w)*3));
}
