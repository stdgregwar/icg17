#version 330
in vec3 position;
out vec3 texcoords;
out vec3 light_dir;

uniform mat4 VP;
uniform mat4 V;
uniform mat4 iV;
uniform mat4 l_iV[3];

void main() {
    gl_Position = VP * vec4(position, 1.0);
    vec4 ldir = l_iV[0] * vec4(0,0,1,0);

    light_dir = normalize(ldir.xyz);
    texcoords = position.xyz;
}
