#version 330
in vec3 texcoords;
in vec3 light_dir;

out vec3 color;

uniform vec3 l_color;

uniform float l_daytime;

uniform samplerCube skybox;

uniform sampler2D sky;

//#include atmosphere.glsl

void main() {

    vec3 sphere = normalize(texcoords);
    float f = pow(clamp(dot(sphere,light_dir),0,1),1000)-0.5;
    vec3 ox = vec3(0.6,0.9,1)*length(l_color)*0.4;
    float spacef = clamp(dot(vec3(0,0,1),sphere),0,1);
    vec3 space = mix(ox,ox*0.5,spacef);
    color = mix(space,l_color,f);
    vec4 skyColor = texture(sky,vec2(l_daytime,(-1*dot(light_dir,sphere)+1)/2.f));
    color = mix(skyColor.rgb,color,0.4);
    color = mix(texture(skybox,-texcoords.xzy).rgb,color,skyColor.a);
    gl_FragDepth = 0.9999999; //Skybox almost at scene back
}
