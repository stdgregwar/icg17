#version 330

uniform sampler1D color_map;
uniform sampler2D height_map;
uniform sampler2D grass;
uniform sampler2D cliffs;
uniform sampler2D pebbles;
uniform sampler2D sand;
uniform sampler2D snow;
uniform sampler2D noise;
uniform samplerCube skybox;
uniform float time;
uniform float res;
uniform mat4 MV;
uniform mat4 M;
uniform float alpha;

in vec3 view_dir;
in vec3 light_dir;

in vData {
    vec2 uv;
    vec3 normal_m;
    vec3 w_pos;
    float base_color;
} vertex;

const mat4 thresholdMatrix = mat4(
            vec4(1.f,13.f,4.f,16.f),
            vec4(9.f,5.f,12.f,8.f),
            vec4(3.f,15.f,2.f,14.f),
            vec4(11.f,7.f,10.f,6.f)
        ) / 17.f;

bool sdoor(vec2 spos, float alpha) {
    int x = int(gl_FragCoord.x);
    int y = int(gl_FragCoord.y);
    return alpha - thresholdMatrix[x % 4][y % 4] < 0;
}

out vec4 color;

float height(vec2 p) {
    return texture(height_map,p).r;
}

vec3 fdiff(vec2 p) {
    float d = 0.5f/(res*8);
    float hL = height(p + vec2(d,0));
    float hR = height(p - vec2(d,0));
    float hD = height(p + vec2(0,d));
    float hU = height(p - vec2(0,d));

    vec3 norm;
    // deduce terrain normal
    norm.x = hL - hR;
    norm.y = hD - hU;
    norm.z = 1800*d;
    return normalize(norm);
}

vec4 triplanar(vec4 x, vec4 y, vec4 z, vec3 normal) {
    float xf = abs(dot(normal,vec3(1,0,0)));
    float yf = abs(dot(normal,vec3(0,1,0)));
    float zf = pow(abs(dot(normal,vec3(0,0,1))),8);
    float t = 1.f/(xf+yf+zf);
    return x*xf*t+y*yf*t+z*zf*t;
}

void main() {
    if(sdoor(gl_FragCoord.xy,alpha)) discard;
    vec3 n = fdiff(vertex.uv);
    vec3 normal_m = normalize((M*vec4(n,0)).xyz);
    vec3 normal = normalize((MV*vec4(n,0)).xyz);
    vec3 light = normalize(light_dir);
    vec3 view = normalize(view_dir);

    float diff = clamp(dot(normal,light),0,1);
    vec3 ref = reflect(light,normal);
    float spec = pow(clamp(dot(ref,view),0,1),2);
    float no = texture(noise,vertex.w_pos.xy*0.06).r*0.3;
    vec3 b_color = texture(color_map,vertex.base_color+no).rgb;

    float dist = pow(clamp(1-distance(b_color,(vec3(0,1,0))),0,1),0.7);
    color = texture(grass,vertex.w_pos.xy*0.25)*dist*1.1;

    dist = clamp(1-distance(b_color,(vec3(0,0,1))),0,1);
    color += texture(pebbles,vertex.w_pos.xy*0.25)*dist;

    dist = clamp(1-distance(b_color,(vec3(0,1,1))),0,1);
    color += texture(sand,vertex.w_pos.xy*0.25)*dist;

    dist = clamp(1-distance(b_color,(vec3(1,0,0))),0,1);
    color += texture(snow,vertex.w_pos.xy*0.25)*dist*2;

    //float fac = pow(dot(normal_m,vec3(0,0,1)),8);
    vec4 rockx = texture(cliffs,vertex.w_pos.yz*0.125);
    vec4 rocky = texture(cliffs,vertex.w_pos.xz*0.125);
    color = triplanar(rockx,rocky,color,normal_m);

    color *= vec4(0.2,0.3,0.3,1)+vec4(1.1)*diff;
    float fog = exp(-0.0004*gl_FragCoord.z/gl_FragCoord.w);
    color = mix(vec4(1),color,fog);
    color.a = gl_FragCoord.w;
}
