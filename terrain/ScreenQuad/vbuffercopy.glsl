#version 330

in vec2 vpoint;
out vec2 uv;

uniform float res;

void main() {
    uv = vpoint*0.5+vec2(0.5);
    gl_Position = vec4(vpoint,0,1);
}
