#version 330

out vec3 color;

uniform sampler2D mirror;

void main() {
    vec2 size = textureSize(mirror,0)*4;
    color = texture(mirror,gl_FragCoord.xy/size).rgb;
}
