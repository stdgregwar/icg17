#version 330

in vec2 position;
in float shift;

out vData {
    vec2 uv;
    vec3 normal_m;
    vec3 w_pos;
    float base_color;
} vertex;

out vec3 view_dir;
out vec3 light_dir;

uniform sampler2D height_map;

uniform mat4 MVP;
uniform mat4 MV;
uniform mat4 M;
uniform float res;
uniform float time;

const vec3 light_world = vec3(1,3,3);

float height(vec2 p) {
    return texture(height_map,p).r;
}

vec3 fdiff(vec2 p) {
    float d = 0.5f/(res);
    float hL = height(p - vec2(d,0));
    float hR = height(p + vec2(d,0));
    float hD = height(p - vec2(0,d));
    float hU = height(p + vec2(0,d));

    vec3 norm;
    // deduce terrain normal
    norm.x = hL - hR;
    norm.y = hD - hU;
    norm.z = 1800*d;
    return normalize(norm);
}

void main() {
    vertex.uv = (position);
    float tres = res;
    vertex.uv*=1-(2/tres); //Align with texture margin
    vertex.uv+=vec2(1/tres);

    float value = height(vertex.uv);
    vertex.base_color = value*0.04;

    vec3 pos_3d = vec3(position.x,position.y,value*0.1+shift);
    vertex.w_pos = (M*vec4(pos_3d,1)).xyz;

    vec3 n = fdiff(vertex.uv);
    vertex.normal_m = normalize((M*vec4(n,0)).xyz);

    view_dir = normalize((MV*vec4(pos_3d,1.0)).xyz);
    light_dir = (MV*vec4(light_world,0)).xyz;

    gl_ClipDistance[0] = dot(vec4(vertex.w_pos,1),vec4(0,0,1,0.1));
    gl_Position = MVP * vec4(pos_3d,1.0);
}
