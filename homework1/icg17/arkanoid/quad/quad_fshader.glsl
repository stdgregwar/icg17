#version 330

in vec2 uv;

out vec4 color;

uniform sampler2D tex;

void main() {
    vec4 texc = texture(tex,uv).rgba;
    float th = 0.9f;
    if(texc.r > th && texc.r > th && texc.g > th && texc.b > th) {
        texc.a = 0;
    }
    color = texc;
}


