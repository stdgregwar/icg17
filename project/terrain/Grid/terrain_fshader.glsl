#version 330

uniform sampler1D color_map;
uniform sampler2D height_map;
uniform sampler2D grass;
uniform sampler2D cliffs;
uniform sampler2D pebbles;
uniform sampler2D sand;
uniform sampler2D snow;
uniform sampler2D noise;
uniform float time;
in vec2 uv;
in vec3 view_dir;
in vec3 normal_mv;
in vec3 normal_m;
in vec3 light_dir;
in vec3 w_pos;
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
    float no = texture(noise,w_pos.xy*0.06).r*0.3;
    vec3 b_color = texture(color_map,base_color+no).rgb;

    float dist = pow(clamp(1-distance(b_color,(vec3(0,1,0))),0,1),0.7);
    color = texture(grass,w_pos.xy*0.25).rgb*dist*1.1;

    dist = clamp(1-distance(b_color,(vec3(0,0,1))),0,1);
    color += texture(pebbles,w_pos.xy*0.25).rgb*dist;

    dist = clamp(1-distance(b_color,(vec3(0,1,1))),0,1);
    color += texture(sand,w_pos.xy*0.25).rgb*dist;

    dist = clamp(1-distance(b_color,(vec3(1,0,0))),0,1);
    color += texture(snow,w_pos.xy*0.25).rgb*dist*2;

    float fac = pow(dot(normal_m,vec3(0,0,1)),4);
    vec3 rock = texture(cliffs,w_pos.xy*0.125).rgb;
    color = mix(rock,color,fac);

    color *= vec3(0.2,0.3,0.3)+vec3(1.1)*diff;//+vec3(1,0.8,0.8)*spec;
    float fog = exp(-0.002*gl_FragCoord.z/gl_FragCoord.w);
    color = mix(vec3(0.7, 0.99, 1),color,fog);
}
