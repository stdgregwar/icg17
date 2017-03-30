#version 330 core
uniform mat4 MVP;
uniform mat4 MV;
uniform float time;
in vec3 vpoint;
in vec2 vtexcoord;

out vec2 uv;
out vec3 point_mv;
out mat4 mv;

mat4 R(float degrees){
    mat4 R = mat4(1);
    float alpha = radians(degrees);
    R[0][0] =  cos(alpha);
    R[0][1] =  sin(alpha);
    R[1][0] = -sin(alpha);
    R[1][1] =  cos(alpha);
    return R;
}
mat4 T(float tx, float ty, float tz){
    mat4 T = mat4(1);
    T[3][0] = tx;
    T[3][1] = ty;
    T[3][2] = tz;
    return T;
}

void main(){
    mat4 tr = R(50*time) * T(1,0,0) * R(50*time);
    gl_Position =  MVP * tr * vec4(vpoint,1); ///< spin-circ
    point_mv = (MV * tr * vec4(vpoint,1)).xyz;
    mv = MV;
//    gl_Position =  MVP * R(50*time)* vec4(vpoint,1); ///< spin
//    gl_Position =  MVP * R(0) * vec4(vpoint,1); ///< still
    uv = vtexcoord;
}
