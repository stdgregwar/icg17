#version 330

uniform mat4 VP;

in vec3 position;
in vec3 normal;
in vec3 color;
in vec2 uv;

out vec3 vColor;
out vec3 vNormal;
out vec2 vUV;

void main() {
    gl_Position = VP*vec4(position,1);
    vColor = color;
    vNormal = normal;
    vUV = uv;
}
