vec3 unpackNormal(sampler2D nmap, vec2 uv) {
    return texture(nmap,uv).xyz*2-vec3(1);
}

vec3 packNormal(vec3 n) {
    return (n+vec3(1))*0.5;
}
