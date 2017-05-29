#version 330
uniform sampler2D gcolor;
uniform sampler2D gdepth;
uniform sampler2D gnormal;

uniform vec3 self_light;
uniform vec3 l_color;
uniform vec3 l_ambient;
uniform mat4 iP;
uniform mat4 iV;
uniform mat4 V;
uniform mat4 MV;

in vec2 uv;
in vec3 light_dir;

out vec3 color;

#include rand.glsl
#include clipspace.glsl
#include shadows.glsl
#include normal.glsl


void main() {
    vec4 gcol = texture(gcolor,uv);
    vec4 gnorm = texture(gnormal,uv);
    vec3 normal = normalize(unpackNormal(gnormal,uv));
    float depth = texture(gdepth,uv).r;
    gl_FragDepth = depth;

    vec3 wpos = worldFrom(uv, depth, iP, iV);
    vec3 eye = (iV*vec4(0,0,0,1)).xyz;
    vec3 view_dir = normalize((V*vec4(wpos,1)).xyz);

    float diff = clamp(dot(normal,light_dir),0,1);
    vec3 ref = reflect(light_dir,normal);
    float spec = clamp(pow(clamp(dot(ref,view_dir),0,1),gnorm.a*64)*gcol.a,0,1);
    float shad = diffFromShadows(vec4(wpos,1), 1, normal, light_dir);
    diff *= shad;
    spec *= shad;

    float dist = distance(eye,wpos);
    vec3 ddir = normalize(eye-wpos);
    float sdiff = clamp(dot(normal,ddir),0,1);
    float att = clamp(1000.f/pow(dist,2),0,1)*0.5;

    //color = s_coords.rgb;
    color = gcol.rgb*(l_ambient+diff*l_color)+spec*l_color+self_light*att*sdiff;
    //color = vec3(att);
    //color = normal;
    //color = mix(color,debug(vec4(wpos,1)),0.125f);
}
