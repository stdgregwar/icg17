#version 330

in vec3 vpoint;
in vec2 vtexcoord;
out vec3 w_pos;
uniform mat4 M;


void main() {
    vec4 point = vec4(vpoint, 1.0);
    w_pos = (M*point).xyz;
    gl_Position = point;
}
