#version 330

uniform sampler2D diffuse;
uniform sampler2D overlay;

in vec2 uv;

out vec3 color;

void main(void)
{
    color = texture(diffuse,uv).rgb + texture(overlay,uv).rgb;
}
