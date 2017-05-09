#version 330
in vec3 texcoords;
out vec3 color;

uniform samplerCube skybox;

void main() {
    color = texture(skybox, texcoords).rgb;
    if(length(color) < 1.2f)
	color = vec3(0);
}
