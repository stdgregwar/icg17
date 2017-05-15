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
uniform mat4 V;
uniform float alpha;

uniform vec3 l_color;
uniform sampler2DShadow shadowmap;

in vec3 view_dir;
in vec3 light_dir;
in vec4 shadow_coord;

in vData {
    vec2 uv;
    vec3 normal_m;
    vec3 w_pos;
    float base_color;
} vertex;

#include sdoor.glsl

#include rand.glsl

out vec4 color;

float height(vec2 p) {
    return texture(height_map,p).r;
}

#include utils.glsl

#include shadows.glsl

void main() {
    if(sdoor(gl_FragCoord.xy,alpha)) discard;
    vec3 n = fdiff(vertex.uv);
    vec3 normal_m = normalize((M*vec4(n,0)).xyz);
    vec3 normal = normalize((MV*vec4(n,0)).xyz);
    vec3 gnormal = normalize((MV*vec4(vertex.normal_m,0)).xyz);
    //normal = gnormal;
    vec3 light = normalize(light_dir);
    vec3 view = normalize(view_dir);

    float diff = clamp(dot(normal,light),0,1);
    vec3 ref = reflect(light,normal);
    float spec = pow(clamp(dot(ref,view),0,1),2);
    float no = texture(noise,vertex.w_pos.xy*0.06).r*0.3;
    no-= texture(noise,vertex.w_pos.xy*0.001).r*0.6;
    vec3 b_color = texture(color_map,vertex.base_color+no).rgb;

    float dist = pow(clamp(1-distance(b_color,(vec3(0,1,0))),0,1),0.7);
    color = texture(grass,vertex.w_pos.xy*0.25)*dist*1.5;

    dist = clamp(1-distance(b_color,(vec3(0,0,1))),0,1);
    color += texture(pebbles,vertex.w_pos.xy*0.25)*dist;

    dist = clamp(1-distance(b_color,(vec3(0,1,1))),0,1);
    color += texture(sand,vertex.w_pos.xy*0.25)*dist;

    dist = clamp(1-distance(b_color,(vec3(1,0,0))),0,1);
    color += texture(snow,vertex.w_pos.xy*0.25)*dist*2;

    //float fac = pow(dot(normal_m,vec3(0,0,1)),8);
    vec4 rockx = texture(cliffs,vertex.w_pos.yz*0.025);
    vec4 rocky = texture(cliffs,vertex.w_pos.xz*0.025);
    color = triplanar(rockx,rocky,color,normal_m);

    diff = diffFromShadows(shadowmap,shadow_coord,diff, normal, light);

    color *= vec4(0.2,0.3,0.3,1)+vec4(l_color,1)*diff;
    float fog = clamp(exp(7-0.002*gl_FragCoord.z/gl_FragCoord.w),0,1);
    //color = mix(vec4(1),color,fog);
    if(sdoor(gl_FragCoord.xy,fog)) discard;
    color.a = gl_FragCoord.w;
 }

