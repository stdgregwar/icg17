#version 330

in vec2 vpoint;
out vec3 w_pos;

uniform float res;
uniform mat4 M;


void main() {
    vec4 point = vec4(vpoint,0,1.0);
    vec2 spoint = (vpoint + vec2(1,1))*0.5f;
    spoint*=1+2/res;
    spoint-=1/res;
    vec4 cpoint = vec4(spoint,0,1);
    w_pos = (M*cpoint).xyz;
    gl_Position = point;
}
