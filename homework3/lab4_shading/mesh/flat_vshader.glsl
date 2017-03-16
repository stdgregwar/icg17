#version 330

in vec3 vpoint;

out vec4 vpoint_mv;
out vec3 light_dir, view_dir;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;
uniform vec3 light_pos;

void main() {
    mat4 MV = view * model;
    vpoint_mv = MV * vec4(vpoint, 1.0);
    vec3 w_pos = (model*vec4(vpoint,1)).xyz;
    gl_Position = projection * vpoint_mv;

    //>>>>>>>>>> TODO >>>>>>>>>>>
    // TODO 4.1: Flat shading.
    // 1) compute the light direction light_dir.
    // 2) compute the view direction view_dir.
    //<<<<<<<<<< TODO <<<<<<<<<<<

    light_dir =  light_pos-w_pos;
    view_dir = vpoint_mv.xyz;
}
