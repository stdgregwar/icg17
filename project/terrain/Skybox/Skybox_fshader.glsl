#version 330
in vec3 texcoords;
out vec4 color;

uniform samplerCube skybox;

void main() {
    color = texture(skybox, texcoords);
}
