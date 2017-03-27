#version 330 core
uniform sampler2D tex;
uniform sampler2D frame_col;

in vec2 uv;
in vec3 point_mv;
in mat4 mv;
out vec3 color;

void main(){
    vec2 size = textureSize(frame_col,0);
    vec3 dx = dFdx(point_mv);
    vec3 dy = dFdy(point_mv);
    vec3 normal = normalize(cross(dx,dy));
    float v = (uv.x+uv.y)*40;
    vec3 vnorm = vec3(cos(v)*50,sin(v)*50,1);
    mat3 tspace = mat3(dx,dy,normal);

    normal = tspace * vnorm;
    //normal = normalize((mv*vec4(normal,0)).xyz);
    vec3 refl = reflect(vec3(0,0,1),normal);
    color = texture(frame_col, gl_FragCoord.xy/size+refl.xy*0.03).rgb;
}
