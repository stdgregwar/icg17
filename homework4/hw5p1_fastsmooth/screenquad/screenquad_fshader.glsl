#version 330

in vec2 uv;

out vec3 color;

uniform sampler2D tex;
uniform float tex_width;
uniform float tex_height;

#define KS 20
uniform vec2 dir;
uniform float[KS] kernel;

float rgb_2_luma(vec3 c) {
    return 0.3*c[0] + 0.59*c[1] + 0.11*c[2];
}

void main() {
    float tot_weight = 0;
    vec3 tot_image = vec3(0.0);
    for(int i = 0; i < KS; i++) {
        vec3 image = texture(tex, uv+i*dir/vec2(tex_width,tex_height)).rgb;
        float weight = kernel[i];
        tot_image += weight * image;
        tot_weight += weight;
    }
    color = tot_image/tot_weight;
}

