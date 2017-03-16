#version 330

uniform vec3 La;
uniform vec3 Ld;
uniform vec3 Ls;

uniform vec3 ka;
uniform vec3 kd;
uniform vec3 ks;
uniform float alpha;

in vec3 normal_mv;
in vec3 light_dir;
in vec3 view_dir;


out vec3 color;

uniform sampler1D tex1D;

void main() {
    color = vec3(0.0,0.0,0.0);
    vec3 normal = normalize(normal_mv);
    vec3 light = normalize(light_dir);
    vec3 view = normalize(view_dir);

    ///>>>>>>>>>> TODO >>>>>>>>>>>
    /// TODO 2.2: Toon shading.
    /// 1) compute ambient term.
    /// 2) compute diffuse term using the texture sampler tex.
    /// 3) compute specular term using the texture sampler tex.
    ///<<<<<<<<<< TODO <<<<<<<<<<<
    vec3 r = reflect(light,normal);
    vec3 ambiant = ka*La;
    vec3 diffuse = kd*texture(tex1D, clamp(dot(normal,light),0,1)).r*Ld;
    vec3 specular = ks*texture(tex1D, clamp(pow(dot(r,view),alpha),0,1)).r* Ls;
    color = ambiant+ diffuse +specular;
}
