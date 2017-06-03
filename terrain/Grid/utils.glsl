#include clipspace.glsl
#include normal.glsl

vec3 fdiff(vec2 p) {
    float d = 0.5f/(res);
    float hL = height(p - vec2(d,0));
    float hR = height(p + vec2(d,0));
    float hD = height(p - vec2(0,d));
    float hU = height(p + vec2(0,d));

    vec3 norm;
    // deduce terrain normal
    norm.x = hL - hR;
    norm.y = hD - hU;
    norm.z = 1500*d;
    return normalize(norm);
}

vec3 getTriPlanarBlend(vec3 _wNorm){
        // in wNorm is the world-space normal of the fragment
        float p = 1/2.f;
	vec3 bl = abs( _wNorm )*vec3(1,1,0.5);
	bl = normalize(max(bl, 0.00001)); // Force weights to sum to 1.0
	float b = (bl.x + bl.y + bl.z);
	bl /= vec3(b, b, b);
	return bl;
}

vec3 triplanar(vec3 x, vec3 y, vec3 z, vec3 normal) {
    vec3 blend = getTriPlanarBlend(normalize(normal+vec3(0,0,2)));
    return x*blend.x+y*blend.y+z*blend.z;
}
