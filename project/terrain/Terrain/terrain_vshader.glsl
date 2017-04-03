#version 330

in vec2 position;

out vec2 uv;
out vec3 normal_mv;
out vec3 view_dir;
out vec3 light_dir;
out vec3 base_color;

uniform sampler2D height_map;
uniform sampler1D color_map;

uniform mat4 MVP;
uniform mat4 MV;

const vec3 light_world = vec3(1,1,1);

float height(vec2 p) {
    return texture(height_map,p).r*1;
}

vec3 fdiff(vec2 p) {
    float d = 1.f/2048;
    float hL = height(p - vec2(d,0));
    float hR = height(p + vec2(d,0));
    float hD = height(p - vec2(0,d));
    float hU = height(p + vec2(0,d));

    vec3 norm;
    // deduce terrain normal
    norm.x = hL - hR;
    norm.y = hD - hU;
    norm.z = 100*d;
    return normalize(norm);
}

void main() {
    uv = (position + vec2(1.0, 1.0)) * 0.2;
    float value = height(uv);
    base_color = texture(color_map,value/2.f).rgb;

    vec3 pos_3d = vec3(position.x,position.y,value*0.1);
    normal_mv = normalize((MV*vec4(fdiff(uv),0)).xyz);
    view_dir = normalize((MV*vec4(pos_3d,1.0)).xyz);
    light_dir = (MV*vec4(light_world,0)).xyz;
    gl_Position = MVP* vec4(pos_3d,1.0);
}
