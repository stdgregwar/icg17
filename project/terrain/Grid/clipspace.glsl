vec3 normHom(vec4 v) {
    return v.xyz/v.w;
}

bool clipSpace(vec3 v) {
    return abs(v.x) < 1 && abs(v.y) < 1 && abs(v.z) < 1;
}
