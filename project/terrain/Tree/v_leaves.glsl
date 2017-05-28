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

void main() {
    vertex.pos = position;
    vertex.color = color;
    vertex.size = size;
    vec4 eye = iV * vec4(0,0,0,1);
    vertex.view_dir = normalize(position-eye.xyz);
}
