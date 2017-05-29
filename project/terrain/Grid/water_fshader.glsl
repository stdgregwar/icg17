#version 330

uniform float time;

out vec3 color;
in vec3 w_pos;
in vec4 pos_3d;
in vec2 uv;
in vec3 light_dir;

uniform float res;
uniform mat4 MV;
uniform sampler2D mirror;
uniform sampler2D waterNormal;
uniform sampler2D refract_col;
uniform sampler2D refract_depth;
uniform sampler2D height_map;
uniform mat4 iV;
uniform mat4 V;
uniform mat4 iP;
uniform vec3 l_color;

float height(vec2 p) {
    vec2 bias = p;
    float tres = res+2;
    bias*=1-(2/tres); //Align with texture margin
    bias+=vec2(1/tres);
    return texture(height_map,bias).r;
}

const mat4 thresholdMatrix = mat4(
            vec4(1.f,13.f,4.f,16.f),
            vec4(9.f,5.f,12.f,8.f),
            vec4(3.f,15.f,2.f,14.f),
            vec4(11.f,7.f,10.f,6.f)
        ) / 17.f;

bool sdoor(vec2 spos, float alpha) {
    int x = int(gl_FragCoord.x);
    int y = int(gl_FragCoord.y);
    return alpha - thresholdMatrix[x % 4][y % 4] < 0;
}


#include normal.glsl
#include clipspace.glsl
#include rand.glsl
#include shadows.glsl

/// Compute chromatic aberration given deltas in some directions
vec3 chromatic(sampler2D tex,vec2 base, vec2 main,vec2 r, vec2 g,vec2 b, float f) {
    vec3 color;
    color.r = texture(tex,base+(main+r*0.01)*f).r;
    color.g = texture(tex,base+(main+g*0.01)*f).g;
    color.b = texture(tex,base+(main+b*0.01)*f).b;
    return color;
}

void main() {
    vec3 view_dir = -normalize((MV*pos_3d).xyz);
    vec3 light = normalize(light_dir);
    vec2 size = textureSize(mirror,0)*2;
    vec2 msize = textureSize(refract_col,0);
    vec2 screenUV = gl_FragCoord.xy/msize;
    vec3 view = normalize(view_dir);
    float disp = sin(time*5+sin(w_pos.x+w_pos.y));
    float ttime = time * 0.1;
    vec3 normal = unpackNormal(waterNormal,w_pos.xy*0.03+vec2(1,-0.453)*ttime);
    normal += unpackNormal(waterNormal,w_pos.xy*0.01+vec2(1,1)*ttime);
    normal += unpackNormal(waterNormal,w_pos.xy*0.112+vec2(0.12,0.45)*4*ttime);
    normal += vec3(0,0,1);
    normal = normalize(normal);
    //normal = vec3(0,0,1);
    vec3 v_normal = normalize((V*vec4(normal,0)).xyz);
    float w_depth = texture(refract_depth,screenUV).r;
    vec3 base_wpos = worldFrom(screenUV,w_depth,iP,iV);

    float dist = distance(base_wpos, w_pos);


    float frebias = -1;
    float frenelpow = 2;
    float fre = 1-dot(view_dir,v_normal)*0.5;
    fre = 1-clamp(frebias+(1-frebias)*pow(fre,frenelpow),0,1);

    float bord = clamp(dist*0.02,0,1);
    float fac = clamp(bord*4,0,1);

    vec3 flat_normal_view = normalize((V*vec4(0,0,1,0)).xyz);

    vec3 rwon = reflect(view_dir,flat_normal_view);
    vec3 rwn = reflect(view_dir,v_normal);
    vec3 rdiff = rwn-rwon; //Making diff of reflection with and without normal mapping to estimate displace in reflect lookup
    vec3 refl = texture(mirror,gl_FragCoord.xy/size+rdiff.xy*0.07*fac).rgb;

    float fog2 = clamp(exp(7-0.0002*gl_FragCoord.z/gl_FragCoord.w),0,1);
    if(sdoor(gl_FragCoord.xy,fog2)) discard;

    //vec3 waterFog = mix(vec3(0.7, 0.99, 1),vec3(0.1,0.2,0.2),fog2);
    vec3 waterFog = vec3(0.1,0.2,0.2); //Compute underwater fog color
    float ior = 1;
    vec3 rewon = refract(view_dir,flat_normal_view,ior);
    vec3 rewn = refract(view_dir,v_normal,ior);
    vec3 rediff = rewn-rewon; //Making diff of refraction with and without normal mapping to estimate displace in refract lookup
    vec3 refr = mix(chromatic(refract_col,screenUV,rediff.xy,vec2(0,1),vec2(-1,0),vec2(1,0),fac*0.05).rgb,waterFog,bord);
    //color = mix(refr,refl,fre*0.25);

    color = mix(refl,refr,fre); //Mix refl/refr using frenel factor

    float shad = diffFromShadows(vec4(w_pos,1), 1, normal, light);

    const float alpha = 20;
    float spec = pow(clamp(dot(rwn,light),0,1),alpha)*shad;
    color = mix(color,l_color,spec); //Adding specular refl
    color = mix(refr,color,bord); //Remix refraction at spots where water is shallow to avoid hard edges

    //color = vec3(bord);

    //float fog2 = exp(-0.00008*gl_FragCoord.z/gl_FragCoord.w);
    //color = mix(vec3(0.7, 0.99, 1),color,fog2);
    //color = refr;
    //color = nrefl;
    //color = normal;
    //color = vec4(texture(refract_depth,).r);
    //color = (rediff+vec3(1))*0.5;
}
