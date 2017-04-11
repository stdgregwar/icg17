#version 330

in vec2 position;
in float shift;


out vec3 w_pos;
out vec2 uv;
out vec4 pos_3d;

uniform mat4 MVP;
uniform mat4 MV;
uniform mat4 M;
uniform float res;
uniform float time;

void main() {
    gl_Position = MVP* vec4(position,shift,1.0);
    uv = position;
    pos_3d = vec4(position,shift,1);
    w_pos = (M * pos_3d).xyz;

}
