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

vec3 baseColor(vec2 p) {
    float heightvalue = texture(height_map,p).r;
    float red = 0.0f;
    float green = 0.0f;
    float blue = 0.0f;
    if(heightvalue >= 0.0f && heightvalue <= 0.5f) {
    // interpolate between (1.0f, 0.0f, 0.0f) and (0.0f, 1.0f, 0.0f)
    green = heightvalue / 128.0f;
    red = 1.0f - green;
    blue = 0.0f;

    } else if(heightvalue > 0.5f && heightvalue <= 1.0f ) {
    // interpolate between (0.0f, 1.0f, 0.0f) and (0.0f, 0.0f, 1.0f)
    red = 0.0f;
    blue = (heightvalue - 127) / 128.0f;
    green = 1.0f - blue;
    }
    return vec3(red,blue,green);
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
    norm.z = 50*d;
    return normalize(norm);
}

void main() {
    uv = (position + vec2(1.0, 1.0)) * 0.2;
    float value = height(uv);
    base_color = texture(color_map,value/2.f).rgb;
//    base_color = vec3(1.0,0.0,0.0);
//    if(value < 1.5f) {
//        base_color = vec3(0.0,0.4,1.0);
//    } else if(value < 2.5f){
//        base_color = vec3(1.0,1.0,0.0);
//    } else if (value < 3.0f) {
//        base_color = vec3(0.31, 0.73, 0.31);
//    } else {
//        base_color = vec3(0.84, 0.84, 0.84);
//    }
    vec3 pos_3d = vec3(position.x,position.y,value*0.1);
    normal_mv = fdiff(uv);
    view_dir = normalize((MV*vec4(pos_3d,1.0)).xyz);
    light_dir = (MV*vec4(light_world,0)).xyz;
    gl_Position = MVP* vec4(pos_3d,1.0);
}
