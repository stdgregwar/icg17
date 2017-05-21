const int cascadeCount = 3;

uniform sampler2DShadow cascades[cascadeCount];

uniform mat4 l_VP[cascadeCount];
uniform mat4 l_V[cascadeCount];
uniform mat4 l_iV[cascadeCount];
uniform mat4 l_P[cascadeCount];

float shadow_val(vec4 wpos) {
    for(int i = 2; i > -1; i--) {
	vec4 shadow_coord = l_VP[i]*wpos;
	if(clipSpace(shadow_coord.xyz)) {
	    vec3 fcoords = vec3(shadow_coord.xy*0.5+vec2(0.5),(shadow_coord.z+1)*0.5);
	    return texture(cascades[i],fcoords);
	}
    }
    return 1.f;
}

float diffFromShadows(vec4 wpos, float diff, vec3 normal, vec3 light) {

    float bias = max(0.005 * (1.0 - dot(normal, light)), 0.0005);
    vec4 shadow_coord = l_VP[0]*wpos;
    float dfac = 1-clamp(-3+length(shadow_coord.xy)*2,0,1);
    for(int i = 2; i > -1; i--) {
	vec4 shadow_coord = l_VP[i]*wpos;
	if(clipSpace(shadow_coord.xyz)) {
	    vec3 fcoords = vec3(shadow_coord.xy*0.5+vec2(0.5)+0.001*rand2(gl_FragCoord.xy),(shadow_coord.z+1)*0.5-bias/(i+1));
	    float d = texture(cascades[i],fcoords);
	    //d = texture(shadowmap,shadow_coord.xyz);
	    float nfac = dot(normal,light);

	    //d += 0.5;
	    //d *= (1-nfac*0.5);
	    //d *= 1-dfac;
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
