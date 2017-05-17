#version 330

uniform sampler2D buffer_color;
uniform sampler2D buffer_depth;

uniform sampler2DShadow shadowmap;

uniform float width;
uniform float height;

uniform mat4 iP;
uniform mat4 iV;
uniform mat4 l_VP;
uniform vec3 l_color;

in vec2 uv;
out vec3 color;

#include rand.glsl
#include clipspace.glsl
#include shadows.glsl

const float maxdiff = 10;

void main(void) {
    float depth = texture(buffer_depth,uv).r;
    float d = 0;
    const int max = 80;
    float bias = 0.003*depth;
    vec3 last = worldFrom(uv,depth,iP,iV);
    vec3 eye = worldFrom(uv,0,iP,iV);
    for(int i = 0; i < max; i++) {
	float fac = float(i)/max;
	float dep = depth*pow(1.00005,-i);
	//float dep = mix(0.9,depth,fac);
	vec4 wpos = vec4(worldFrom(uv,dep,iP,iV),1);
	//vec4 wpos = vec4(mix(last,eye,fac),1);
	vec4 shadow_coords = l_VP*wpos;
	float step = distance(last,wpos.xyz);
	d += step*0.9*(shadow_val(shadowmap,shadow_coords-vec4(0,0,bias,0)));
	last = wpos.xyz;
    }
    d = clamp(d,0,maxdiff);
    color = (d/max)*l_color;
    //color = 0.003*last;
    //color = vec3(depth*0.1);

}
