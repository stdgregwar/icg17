vec3 normHom(vec4 v) {
    return v.xyz/v.w;
}

bool clipSpace(vec3 v) {
    return abs(v.x) < 1 && abs(v.y) < 1 && abs(v.z) < 1;
}

vec3 worldFrom(vec2 uv, float depth, mat4 iP, mat4 iV) {
        float z = depth * 2.0 - 1.0;

        vec4 clipSpacePosition = vec4(uv * 2.0 - 1.0, z, 1.0);
        vec4 viewSpacePosition = iP * clipSpacePosition;

        // Perspective division
        viewSpacePosition /= viewSpacePosition.w;

        vec4 worldSpacePosition = iV * viewSpacePosition;

        return worldSpacePosition.xyz;
}
