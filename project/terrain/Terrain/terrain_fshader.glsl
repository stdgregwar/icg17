#version 330

uniform sampler1D color_map;
uniform sampler2D height_map;
uniform float time;
in vec2 uv;
in vec3 view_dir;
in vec3 normal_mv;
in vec3 light_dir;
in float base_color;

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
    float spec = pow(clamp(dot(ref,view),0,1),2);
    color = texture(color_map,base_color).rgb;
    color *= vec3(0.1,0.2,0.2)+vec3(0.9)*diff;//+vec3(1,0.8,0.8)*spec;
    float fog = exp(-0.002*gl_FragCoord.z/gl_FragCoord.w);
    color = mix(vec3(0.7, 0.99, 1),color,fog);
}
