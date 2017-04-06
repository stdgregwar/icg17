#version 330

in vec2 position;
in float shift;

out vec2 uv;
out vec3 normal_mv;
out vec3 normal_m;
out vec3 w_pos;
out vec3 view_dir;
out vec3 light_dir;
out float base_color;

uniform sampler2D height_map;

uniform mat4 MVP;
uniform mat4 MV;
uniform mat4 M;
uniform float res;
uniform float time;

const vec3 light_world = vec3(1,1,1);

float height(vec2 p) {
    return texture(height_map,p).r*1;
}

vec3 fdiff(vec2 p) {
    float d = 0.5f/res;
    float hL = height(p - vec2(d,0));
    float hR = height(p + vec2(d,0));
    float hD = height(p - vec2(0,d));
    float hU = height(p + vec2(0,d));

    vec3 norm;
    // deduce terrain normal
    norm.x = hL - hR;
    norm.y = hD - hU;
    norm.z = 400*d;
    return normalize(norm);
}

void main() {
    uv = (position);
    float tres = res+2;
    uv*=1-(2/tres); //Align with texture margin
    uv+=(1/tres);

    float value = height(uv);
    base_color = value*0.09;

    vec3 pos_3d = vec3(position.x,position.y,value*0.1+shift);
    w_pos = (M*vec4(pos_3d,1)).xyz;
    vec3 n = fdiff(uv);
    normal_mv = normalize((MV*vec4(n,0)).xyz);
    normal_m = normalize((M*vec4(n,0)).xyz);
    view_dir = normalize((MV*vec4(pos_3d,1.0)).xyz);
    light_dir = (MV*vec4(light_world,0)).xyz;
    gl_Position = MVP* vec4(pos_3d,1.0);
}
