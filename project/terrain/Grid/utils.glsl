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



vec3 triplanar(vec3 x, vec3 y, vec3 z, vec3 normal) {
    float xf = abs(dot(normal,vec3(1,0,0)));
    float yf = abs(dot(normal,vec3(0,1,0)));
    float zf = pow(abs(dot(normal,vec3(0,0,1))),3);
    float t = 1.f/(xf+yf+zf);
    return x*xf*t+y*yf*t+z*zf*t;
}
