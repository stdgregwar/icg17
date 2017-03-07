#version 330

in vec2 uv;
in vec4 position;

out vec3 color;

uniform sampler1D colormap;

void main() {
    float sinx = sin(uv.x*3.1415*10);
    float siny = sin(uv.y*3.1415*10);
    float val = sinx*siny*0.5+0.5;

    color = texture(colormap, val).rgb;
}


