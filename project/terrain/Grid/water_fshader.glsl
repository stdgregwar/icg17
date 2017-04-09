#version 330

uniform float time;

out vec4 color;
in vec3 w_pos;
in vec2 uv;

uniform sampler2D mirror;
uniform sampler2D waterNormal;
uniform sampler2D height_map;
uniform sampler2D behind_color;
uniform sampler2D behind_depth;

void main() {
    vec2 size = textureSize(mirror,0)*4;
    float disp = sin(time*5+sin(w_pos.x+w_pos.y));
    float ttime = time * 0.1;
    vec3 normal = texture(waterNormal,w_pos.xy*0.03+vec2(1,-1)*ttime).rgb;
    normal += texture(waterNormal,w_pos.xy*0.03+vec2(1,1)*ttime).rgb;
    normal = normalize(normal - vec3(0.5));
    float height = texture(height_map,uv).r;
    float fac = 1-clamp(abs(height),0,1);
    color = texture(mirror,gl_FragCoord.xy/size+normal.xy*0.02*(1-fac)) * vec4(1,1,1,0.7-0.7*fac);
    //color = vec4(fac,fac,fac,1);
}
