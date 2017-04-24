#version 330

in vec2 position;
in float shift;


out vec3 w_pos;
out vec2 uv;
out vec4 pos_3d;

out vec3 light_dir;

uniform mat4 MVP;
uniform mat4 MV;
uniform mat4 M;
uniform float res;
uniform float time;

const vec3 light_world = vec3(1,3,3);

void main() {
    gl_Position = MVP* vec4(position,shift,1.0);
    uv = position;
    pos_3d = vec4(position,shift,1);
    w_pos = (M * pos_3d).xyz;
    light_dir = (MV*vec4(light_world,0)).xyz;
}
