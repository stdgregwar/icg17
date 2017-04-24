#version 330

in vData {
    vec2 uv;
    float alpha;
} vertex;

out vec3 color;

void main(void)
{
    color = vec3(0,1,vertex.alpha);
}
