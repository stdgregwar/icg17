#version 330

uniform sampler2D buffer_color;
uniform sampler2D buffer_depth;

uniform float width;
uniform float height;

uniform mat4 iP;
uniform mat4 iV;
uniform vec3 l_color;

in vec2 uv;
out vec3 color;

#include rand.glsl
#include clipspace.glsl
#include shadows.glsl

const float maxdiff = 30;
const float density = 3;

void main(void) {
    float depth = texture(buffer_depth,uv).r;
    float d = 0;
    const int max = 50;
    float bias = 0.003*depth;
    vec3 base = worldFrom(uv,depth,iP,iV);
    vec3 last = base;
    vec3 eye = (iV * vec4(0,0,0,1)).xyz;
    for(int i = 0; i < max; i++) {
	float fac = float(i)/max;

	vec4 wpos = vec4(mix(base,eye,fac),1);
	d += density*(shadow_val(wpos));
	last = wpos.xyz;
    }
    float dist = distance(base,eye);
    float df = 1-clamp(exp(-dist*0.001),0,1);
    //d = clamp(d,0,maxdiff);
    color = df*(d/max)*0.25*l_color;
    //color = debug(vec4(base,1));
}
