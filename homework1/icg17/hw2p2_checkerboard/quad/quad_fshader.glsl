#version 330

in vec2 uv;
in vec4 position;

out vec3 color;

uniform sampler1D colormap;

mat4 R(float degrees){
    mat3 R = mat3(1);
    float alpha = radians(degrees);
    R[0][0] =  cos(alpha);
    R[0][1] =  sin(alpha);
    R[1][0] = -sin(alpha);
    R[1][1] =  cos(alpha);
    return mat4(R);
}

void main() {
    vec4 position_rotated = R(-10) * position;
    float value = position_rotated.x + position_rotated.y;
    float sinx = sin(uv.x*3.1415*10);
    float siny = sin(uv.y*3.1415*10);
    float val = sinx*siny*0.5+0.5;
    //color = vec3(1.0, 0.0, 0.0);
    color = texture(colormap, val).rgb;
}


