#version 330

uniform float time;

out vec3 color;
in vec3 w_pos;
in vec3 view_dir;
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
    return (texture(tex,coords).rgb-vec3(0.5))*2;
}

void main() {
    vec2 size = textureSize(mirror,0)*2;
    vec2 msize = textureSize(refract_col,0);
    vec2 screenUV = gl_FragCoord.xy/msize;
    vec3 view = normalize(view_dir);
    float disp = sin(time*5+sin(w_pos.x+w_pos.y));
    float ttime = time * 0.1;
    vec3 normal = normalFromTex(waterNormal,w_pos.xy*0.03+vec2(1,-0.453)*ttime);
    normal *= normalFromTex(waterNormal,w_pos.xy*0.01+vec2(1,1)*ttime);
    normal += vec3(0,0,5);
    normal = normalize(normal);
    //normal = vec3(0,0,1);
    vec3 v_normal = normalize((MV*vec4(normal,0)).xyz);

    float w_depth = texture(refract_depth,screenUV).r/texture(refract_col,screenUV).a;
    float fogexp = -0.008;
    float wfog = exp(fogexp*w_depth);
    float fog = exp(fogexp*gl_FragCoord.z/gl_FragCoord.w);
    float diff = 1-(fog-wfog)*2/fog;


    float frebias = -0.2;
    float frenelpow = 1.3;
    float fre = 1-dot(view_dir,v_normal);
    fre = max(0,frebias+(1-frebias)*pow(fre,frenelpow));

    float bord = abs(-(height(uv)));
    float fac = clamp(bord,0,1);

    vec3 refl = texture(mirror,gl_FragCoord.xy/size+normal.xy*0.2*fac).rgb;
    vec3 refr = texture(refract_col,gl_FragCoord.xy/msize+normal.xy*fac*0.2).rgb;
    //color = mix(refr,refl,fre*0.25);



    color = mix(refl,refr*vec3(0.7,0.7,0.8)*diff,fre*0.25);
    color = mix(refr,color,clamp(bord*4,0,1));

    //float fog2 = exp(-0.0008*gl_FragCoord.z/gl_FragCoord.w);
    //color = mix(vec3(0.7, 0.99, 1),color,fog2);
    //color = refr;
    //color = nrefl;
    //color = normal;
    //color = vec4(texture(refract_depth,).r);
    //color = vec3(diff);
}
