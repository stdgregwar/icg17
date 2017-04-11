#version 330

uniform float time;

out vec3 color;
in vec3 w_pos;
in vec4 pos_3d;
in vec2 uv;

uniform float res;
uniform mat4 MV;
uniform sampler2D mirror;
uniform sampler2D waterNormal;
uniform sampler2D refract_col;
uniform sampler2D refract_depth;
uniform sampler2D height_map;

float height(vec2 p) {
    vec2 bias = p;
    float tres = res+2;
    bias*=1-(2/tres); //Align with texture margin
    bias+=vec2(1/tres);
    return texture(height_map,bias).r;
}

vec3 normalFromTex(sampler2D tex, vec2 coords) {
    return (texture(tex,coords).rgb*2)-1;
}

void main() {
    vec3 view_dir = -normalize((MV*pos_3d).xyz);
    vec2 size = textureSize(mirror,0)*2;
    vec2 msize = textureSize(refract_col,0);
    vec2 screenUV = gl_FragCoord.xy/msize;
    vec3 view = normalize(view_dir);
    float disp = sin(time*5+sin(w_pos.x+w_pos.y));
    float ttime = time * 0.1;
    vec3 normal = normalFromTex(waterNormal,w_pos.xy*0.03+vec2(1,-0.453)*ttime);
    normal += normalFromTex(waterNormal,w_pos.xy*0.01+vec2(1,1)*ttime);
    normal += normalFromTex(waterNormal,w_pos.xy*0.212+vec2(0.12,0.45)*4*ttime);
    normal += vec3(0,0,60);
    normal = normalize(normal);
    //normal = vec3(0,0,1);
    vec3 v_normal = normalize((MV*vec4(normal,0)).xyz);

    float w_depth = texture(refract_depth,screenUV).r/texture(refract_col,screenUV).a;

    float fogexp = -0.008;
    float wfog = exp(fogexp*w_depth);
    float fog = exp(fogexp*gl_FragCoord.z/gl_FragCoord.w);
    float diff = 1-(fog-wfog)/fog;


    float frebias = 0;
    float frenelpow = 1;
    float fre = 1-dot(view_dir,v_normal);
    fre = 1-clamp(frebias+(1-frebias)*pow(fre,frenelpow),0,1);

    float bord = abs(-(height(uv)));
    bord = clamp(1-pow(diff,6),0,1);
    float fac = clamp(bord,0,1);

    vec3 refl = texture(mirror,gl_FragCoord.xy/size+normal.xy*0.2*fac).rgb;
    vec3 refr = mix(texture(refract_col,gl_FragCoord.xy/msize+normal.xy*fac*0.2).rgb,vec3(0.1,0.2,0.2),bord);
    //color = mix(refr,refl,fre*0.25);

    color = mix(refl,refr,fre);

    color = mix(refr,color,bord);

    float fog2 = exp(-0.00008*gl_FragCoord.z/gl_FragCoord.w);
    color = mix(vec3(0.7, 0.99, 1),color,fog2);
    //color = refr;
    //color = nrefl;
    //color = normal;
    //color = vec4(texture(refract_depth,).r);
    //color = vec3(bord);
}
