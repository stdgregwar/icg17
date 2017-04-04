#version 330

in vec3 vpoint;
in vec2 vtexcoord;
out vec3 w_pos;

uniform float res;
uniform mat4 M;


void main() {
    vec4 point = vec4(vpoint, 1.0);
    point *= 1+(2/res); //Take margin of 1px at each end for normal completness
    point -= (1/res);
    w_pos = (M*point).xyz;
    gl_Position = point;
}
