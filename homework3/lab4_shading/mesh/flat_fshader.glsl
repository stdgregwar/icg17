#version 330

uniform vec3 La;
uniform vec3 Ld;
uniform vec3 Ls;

uniform vec3 ka;
uniform vec3 kd;
uniform vec3 ks;

uniform float alpha;

in vec4 vpoint_mv;
in vec3 light_dir, view_dir;

out vec3 color;

void main() {
    vec3 dx = dFdx(vpoint_mv.xyz);
    vec3 dy = dFdy(vpoint_mv.xyz);
    vec3 normal = normalize(cross(dx,dy));
    vec3 light = normalize(light_dir);
    vec3 view = normalize(view_dir);

    ///>>>>>>>>>> TODO >>>>>>>>>>>
    /// TODO 4.2: Flat shading.
    /// 1) compute triangle normal using dFdx and dFdy
    /// 1) compute ambient term.
    /// 2) compute diffuse term.
    /// 3) compute specular term.
    ///<<<<<<<<<< TODO <<<<<<<<<<<

    float diff = clamp(dot(normal,light),0,1);
    vec3 ref = reflect(light,normal);
    float spec = pow(clamp(dot(ref,view),0,1),alpha);

    color = La*ka+Ld*kd*diff+Ls*ks*spec;
    color = normal;
   // color = Ld*kd*diff;
}
