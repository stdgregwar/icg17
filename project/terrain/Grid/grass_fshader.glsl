#version 330

in vData {
    vec2 uv;
    vec3 normal_m;
    vec3 w_pos;
    float base_color;
} vertex;

out vec3 color;

void main(void)
{
    color = vec3(0,1,0);
}
