float diffFromShadows(sampler2DShadow shadowmap, vec4 shadow_coord, float diff, vec3 normal, vec3 light) {

    float bias = max(0.005 * (1.0 - dot(normal, light)), 0.0005);

    if(clipSpace(shadow_coord.xyz)) {

	vec3 fcoords = vec3(shadow_coord.xy*0.5+vec2(0.5)+0.0006*rand2(gl_FragCoord.xy),(shadow_coord.z+1)*0.5-bias);
	float d = texture(shadowmap,fcoords);
	//d = texture(shadowmap,shadow_coord.xyz);
	float nfac = dot(normal,light);
	float dfac = 1-clamp(-2+length(shadow_coord.xy)*4,0,1);
	//d += 0.5;
	d *= (1-nfac*0.5);
	//d *= 1-dfac;
	return mix(diff, diff*clamp(d,0,1), dfac);
	//color.xyz = vec3(0,1,0);
    }

    return diff;
}
