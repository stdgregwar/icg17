#version 330

in vData {
    vec2 uv;
    vec3 color;
    float alpha;
    vec3 normal;
} vertex;

out vec3 color;

uniform sampler2D grasspatch;
uniform mat4 V;

const mat4 thresholdMatrix = mat4(
            vec4(1.f,13.f,4.f,16.f),
            vec4(9.f,5.f,12.f,8.f),
            vec4(3.f,15.f,2.f,14.f),
            vec4(11.f,7.f,10.f,6.f)
        ) / 17.f;

bool sdoor(vec2 spos, float alpha) {
    int x = int(gl_FragCoord.x);
    int y = int(gl_FragCoord.y);
    return alpha - thresholdMatrix[x % 4][y % 4] < 0;
}

void main(void)
{
    vec4 tex = texture(grasspatch,vertex.uv);
    if(sdoor(gl_FragCoord.xy, vertex.alpha)) discard;
    if(tex.a < 0.8f) discard;
    color = vertex.color*tex.rgb;
}
