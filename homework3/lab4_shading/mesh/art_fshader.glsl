#version 330

uniform sampler2D art_tex;

uniform float alpha;

in vec3 normal_mv;
in vec3 light_dir;
in vec3 view_dir;

out vec3 color;

void main() {
    vec3 normal = normalize(normal_mv);
    vec3 light = normalize(light_dir);
    vec3 view = normalize(view_dir);

    //>>>>>>>>>> TODO >>>>>>>>>>>
    // TODO 1.2: Phong shading.
    // 1) compute ambient term.
    // 2) compute diffuse term.
    // 3) compute specular term.
    // To avoid GPU bug, remove
    // the code above after
    // implementing Phong shading.
    //<<<<<<<<<< TODO <<<<<<<<<<<
    float diff = clamp(dot(normal,light),0,1);
    vec3 ref = reflect(light,normal);
    float spec = pow(clamp(dot(ref,view),0,1),alpha);
    color = texture(art_tex,vec2(diff,spec)).rgb;
}
