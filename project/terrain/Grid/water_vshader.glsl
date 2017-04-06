#version 330

in vec2 position;
in float shift;

uniform sampler2D height_map;

uniform mat4 MVP;
uniform mat4 MV;
uniform mat4 M;
uniform float res;
uniform float time;

void main() {
    gl_Position = MVP* vec4(position,shift,1.0);
    vec4 w_pos = M * vec4(position,shift, 1.0);
    gl_ClipDistance[0] = dot(w_pos, vec4(0.0,0.0,1.0,0.5));
}
