#version 330

in vec2 position;

out vec2 uv;
out vec3 normal_mv;
out vec3 view_dir;
out vec3 light_dir;

uniform sampler2D height_map;

uniform mat4 MVP;
uniform mat4 MV;

const vec3 light_world = vec3(1,1,1);

float height(vec2 p) {
    return texture(height_map,p).r*3;
}

vec3 fdiff(vec2 p) {
    float d = 0.01f;
    vec3 off = vec3(1.0, 1.0, 0.0)*d;
    float hL = height(p - off.xz);
    float hR = height(p + off.xz);
    float hD = height(p - off.zy);
    float hU = height(p + off.zy);

    vec3 norm;
    // deduce terrain normal
    norm.x = hL - hR;
    norm.y = hD - hU;
    norm.z = 0.5;
    return normalize(norm);
}

void main() {
    uv = (position + vec2(1.0, 1.0)) * 0.2;

    float value = height(uv);
    vec3 pos_3d = vec3(position.x,position.y,value*0.1);
    normal_mv = fdiff(uv);
    view_dir = normalize((MV*vec4(pos_3d,1.0)).xyz);
    light_dir = (MV*vec4(light_world,0)).xyz;
    gl_Position = MVP* vec4(pos_3d,1.0);
}
