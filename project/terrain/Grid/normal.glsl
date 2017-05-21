vec3 unpackNormal(sampler2D nmap, vec2 uv) {
    return texture(nmap,uv).xyz*2-vec3(1);
}

vec3 packNormal(vec3 n) {
    return (n+vec3(1))*0.5;
}

mat3 tSpace(vec3 normal, vec3 w_bit) {
    vec3 bit = normalize((MV*vec4(w_bit,0)).xyz);
    vec3 tt = cross(bit,normal);
    bit = cross(normal,tt);
    mat3 ts;
    ts[0] = tt;
    ts[1] = bit;
    ts[2] = normal;
    return transpose(ts);
}
