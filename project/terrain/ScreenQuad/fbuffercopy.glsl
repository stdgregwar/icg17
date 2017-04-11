#version 330

uniform sampler2D buffer_color;
uniform sampler2D buffer_depth;

in vec2 uv;
out vec4 color;

void main(void)
{
    gl_FragDepth = texture(buffer_depth,uv).r;
    color = texture(buffer_color,uv);
}
