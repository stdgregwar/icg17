#version 330

uniform mat4 l_iV[3];
uniform mat4 V;
uniform mat4 MV;

in vec2 vpoint;
out vec2 uv;
out vec3 light_dir;

void main() {
    uv = vpoint*0.5+vec2(0.5);

    vec4 ldir = l_iV[0] * vec4(0,0,1,0);
    light_dir = normalize((V*ldir).xyz);

    gl_Position = vec4(vpoint,0,1);
}
