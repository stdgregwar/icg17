#version 330

in vec2 uv;
in float cfac;
out vec3 color;

uniform sampler2D tex;

void main() {
    color = texture(tex, uv).rgb*cfac+vec3(1)*(1-cfac);
}
