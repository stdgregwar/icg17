#version 330

uniform sampler1D color_map;
uniform sampler2D height_map;
uniform sampler2D moss_alb;
uniform sampler2D moss_nor;
uniform sampler2D rock_alb;
uniform sampler2D rock_nor;
uniform sampler2D pebbles_alb;
uniform sampler2D pebbles_nor;
uniform sampler2D sand;
uniform sampler2D snow;
uniform sampler2D noise;
uniform float time;
uniform float res;
uniform mat4 MV;
uniform mat4 M;
uniform mat4 V;
uniform float alpha;

in vData {
    vec2 uv;
    vec3 normal_m;
    vec3 w_pos;
    float base_color;
} vertex;

#include sdoor.glsl

//#include rand.glsl

layout (location = 0) out vec4 color;
layout (location = 1) out vec4 normal;

float height(vec2 p) {
    return texture(height_map,p).r;
}

#include utils.glsl

void main() {
    if(sdoor(gl_FragCoord.xy,alpha)) discard;
    vec3 n = fdiff(vertex.uv);
    vec3 normal_m = normalize((M*vec4(n,0)).xyz);



    vec3 gnormal = normalize((MV*vec4(vertex.normal_m,0)).xyz);
    //normal.xyz = gnormal;

    vec3 norm = vec3(0,0,1);
    float spec = 0;
    float power = 0;
    float no = texture(noise,vertex.w_pos.xy*0.06).r*0.3;
    no-= texture(noise,vertex.w_pos.xy*0.001).r*0.6;
    vec3 b_color = texture(color_map,vertex.base_color+no).rgb;

    float dist = pow(clamp(1-distance(b_color,(vec3(0,1,0))),0,1),0.7);
    color = texture(moss_alb,vertex.w_pos.xy*0.25);
    norm+= unpackNormal(moss_nor,vertex.w_pos.xy*0.25)*dist;
    spec+=dist*0.1;
    power+=dist;

    dist = clamp(1-distance(b_color,(vec3(0,0,1))),0,1);
    color += texture(pebbles_alb,vertex.w_pos.xy*0.25)*dist;
    norm+= unpackNormal(pebbles_nor,vertex.w_pos.xy*0.25)*dist;
    spec+=dist*0.5;
    power+=dist*0.3;

    dist = clamp(1-distance(b_color,(vec3(0,1,1))),0,1);
    color += texture(sand,vertex.w_pos.xy*0.25)*dist;

    dist = clamp(1-distance(b_color,(vec3(1,0,0))),0,1);
    color += texture(snow,vertex.w_pos.xy*0.25)*dist*2;
    norm+=vec3(0,0,2)*dist;
    spec+=dist;
    power+=dist;

    //float fac = pow(dot(normal_m,vec3(0,0,1)),8);
    vec3 rockx_nor = unpackNormal(rock_nor,vertex.w_pos.yz*0.025);
    vec3 rocky_nor = unpackNormal(rock_nor,vertex.w_pos.xz*0.025);
    vec3 rockx = texture(rock_alb,vertex.w_pos.yz*0.025).rgb;
    vec3 rocky = texture(rock_alb,vertex.w_pos.xz*0.025).rgb;
    color.rgb = triplanar(rockx,rocky,color.rgb,normal_m).rgb;
    norm = triplanar(rockx_nor,rocky_nor,norm,normal_m);
    norm = normalize(norm);
    n = normalize(n+norm*0.2);
    //tnormal = xytspace*norm;
    color.a = clamp(spec,0,1); //This is specular amount
    normal.a = clamp(power,0,1); //This is specular power

    //n.xy *= -1;
    n *= -1;
    vec3 tnormal = normalize((MV*vec4(n,0)).xyz);
    normal.xyz = packNormal(tnormal);

    float fog = clamp(exp(7-0.002*gl_FragCoord.z/gl_FragCoord.w),0,1);
    if(sdoor(gl_FragCoord.xy,fog)) discard;

 }

