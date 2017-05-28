#version 330

uniform sampler2D leaves;

in vData {
    vec2 uv;
    vec3 color;
    vec3 normal;
} vertex;

void main() {
    vec4 tex = texture(leaves,vertex.uv);
    if(tex.a < 0.8f) discard;
}
