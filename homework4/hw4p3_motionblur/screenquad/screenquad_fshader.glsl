#version 330 core
in vec2 uv;
uniform sampler2D colorTex;
uniform sampler2D velocityTex;
out vec4 color;

#define KS 30

void main() {
    /// TODO: use the velocity vector stored in velocityTex to compute the line integral
    /// TODO: use a constant number of samples for integral (what happens if you take too few?)
    /// HINT: you can scale the velocity vector to make the motion blur effect more prominent
    /// HINT: to debug integration don't use the velocityTex, simply assume velocity is constant, e.g. vec2(1,0)

    vec4 tot_image = vec4(0);
    float tot_weight = 0;
    for(int i = 0; i < KS; i++) {
        vec2 speed = texture(velocityTex,uv).xy;
        vec4 image = texture(colorTex, uv+i*speed*0.05);
        tot_image += image;
        tot_weight += 1;
    }
    color = tot_image / tot_weight;
}
