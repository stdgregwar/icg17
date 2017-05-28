#version 330

uniform mat4 iV;

in vec3 position;
in vec3 color;
in float size;

out vData {
    int tex;
    vec3 pos;
    vec3 color;
    vec3 view_dir;
    float size;
} vertex;

#include rand.glsl

void main() {
    vertex.pos = position;
    vertex.color = color;
    vertex.size = size;
    vec4 eye = iV * vec4(0,0,-1,0);
    vertex.view_dir = normalize(eye.xyz);
    vertex.tex =  int((color.b)*4);
}
