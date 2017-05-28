#version 330

uniform sampler2D tree;

in vData {
    vec2 uv;
    vec3 normal;
} vertex;

layout (location = 0) out vec4 color;
layout (location = 1) out vec4 normal;

void main() {
    normal.rgb = vertex.normal;
    normal.a = 0;
    vec4 tex = texture(tree,vertex.uv);
    if(tex.a < 0.8f) discard;
    color.rgb = tex.rgb*1.2;
    color.a = 0;
}
