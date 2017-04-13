#version 330
in vec3 position;
out vec3 texcoords;

uniform mat4 VP;

void main() {
    gl_Position = VP * vec4(position, 1.0);
    texcoords = -position.xzy;
}
