const int cascadeCount = 3;

uniform sampler2DShadow cascades[cascadeCount];

uniform mat4 l_VP[cascadeCount];
uniform mat4 l_V[cascadeCount];
uniform mat4 l_iV[cascadeCount];
uniform mat4 l_P[cascadeCount];

#define val_loop(i,samp) \
    if(i == 0) \
        return texture(samp[0],fcoords);\
    if(i == 1) \
        return texture(samp[1],fcoords);\
    if(i == 2) \
        return texture(samp[2],fcoords);

float shadow_val(vec4 wpos) {
    for(int i = 2; i > -1; i--) {
	vec4 shadow_coord = l_VP[i]*wpos;
	if(clipSpace(shadow_coord.xyz)) {
	    vec3 fcoords = vec3(shadow_coord.xy*0.5+vec2(0.5),(shadow_coord.z+1)*0.5);
	    val_loop(i,cascades);
	}
    }
    return 1.f;
}

#define diff_loop(i,samp,fcoord) \
    float d;\
    if(i == 0) \
        d = texture(samp[0],fcoord);\
    if(i == 1) \
        d = texture(samp[1],fcoord);\
    if(i == 2) \
        d = texture(samp[2],fcoord);

float diffFromShadows(vec4 wpos, float diff, vec3 normal, vec3 light) {

    float bias = max(0.005 * (1.0 - dot(normal, light)), 0.0005);
    vec4 shadow_coord = l_VP[0]*wpos;
    float dfac = 1-clamp(-3+length(shadow_coord.xy)*2,0,1);
    for(int i = 2; i > -1; i--) {
	vec4 shadow_coord = l_VP[i]*wpos;
	if(clipSpace(shadow_coord.xyz)) {
	    vec3 fcoords = vec3(shadow_coord.xy*0.5+vec2(0.5)+0.001*rand2(gl_FragCoord.xy),(shadow_coord.z+1)*0.5-bias/(i*2+1));
	    diff_loop(i,cascades,fcoords);
	    return mix(diff, diff*clamp(d,0,1), dfac);
	    //color.xyz = vec3(0,1,0);
	}
    }
    return diff;
}

vec3 debug(vec4 wpos) {
    vec3 col = vec3(0);
    for(int i = 2; i > -1; i--) {
	vec4 shadow_coord = l_VP[i]*wpos;
	if(clipSpace(shadow_coord.xyz)) {
	    col[i] = 1;
	    return col;
	}
    }
}
