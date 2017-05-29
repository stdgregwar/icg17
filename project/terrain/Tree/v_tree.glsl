#version 330

uniform mat4 VP;
uniform mat4 V;

in vec3 position;
in vec3 normal;
in vec3 color;
in vec2 uv;

out vec3 vColor;
out vec3 vNormal;
out vec2 vUV;
out mat3 tSpace;

void main() {
    gl_Position = VP*vec4(position,1);
    vColor = color;
    vNormal = mat3(V)*normal;
    vec3 bino = cross(normal,vec3(0,0,1));
    vec3 bitg = cross(bino,normal);
    tSpace[0] = normalize(bino);
    tSpace[1] = normalize(bitg);
    tSpace[2] = normal;
    tSpace = mat3(V)*tSpace;
    vUV = uv*vec2(1,8);
}
