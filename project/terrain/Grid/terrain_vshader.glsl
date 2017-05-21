#version 330

in vec2 position;
in float shift;

out vData {
    vec2 uv;
    vec3 normal_m;
    vec3 w_pos;
    float base_color;
} vertex;

uniform sampler2D height_map;

uniform mat4 MVP;
uniform mat4 MV;
uniform mat4 M;
uniform float res;
uniform float time;

float height(vec2 p) {
    return texture(height_map,p).r;
}

#include utils.glsl

void main() {
    vertex.uv = (position);
    float tres = res;
    vertex.uv*=1-(2/tres); //Align with texture margin
    vertex.uv+=vec2(1/tres);

    float value = height(vertex.uv);
    vertex.base_color = value*0.01+0.4;

    vec3 pos_3d = vec3(position.x,position.y,value*0.1+shift);
    vertex.w_pos = (M*vec4(pos_3d,1)).xyz;

    vec3 n = fdiff(vertex.uv);
    vertex.normal_m = normalize((M*vec4(n,0)).xyz);

    gl_ClipDistance[0] = dot(vec4(vertex.w_pos,1),vec4(0,0,1,0.1));
    gl_Position = MVP * vec4(pos_3d,1.0);
}
