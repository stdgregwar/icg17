#version 330

in vec2 position;

out vec2 uv;
out float cfac;

uniform mat4 MVP;
uniform float time;

// Set to false to see nice waves
const bool SINE = true;

float waveform(float f) {
    return -abs(sin(f));
}

float waveHeight(vec2 pos, float angle, float length, float frequency) {
    float phase = 4.f * frequency;
    float theta = dot(vec2(cos(angle),sin(angle)),pos*length);
    return waveform(theta*frequency+time*phase);
}

void main() {
    uv = (position + vec2(1.0, 1.0)) * 0.5;

    // convert the 2D position into 3D positions that all lay in a horizontal
    // plane.
    // TODO 6: animate the height of the grid points as a sine function of the
    // 'time' and the position ('uv') within the grid.
    float height;
    if(SINE){
        height = sin(10*uv.x+10*uv.y+6.28*time)*0.03;
        cfac = 1;
    } else {
        height = 0.3f;
        height += waveHeight(uv,-0.6f,40.f,0.5f)*0.04;
        height += waveHeight(uv,0.1f,65.f,0.76f)*0.02;
        height += waveHeight(uv,0.f,7.f,0.2f)*0.3f;
        cfac = (0.26f - height)*5.f;
        uv+=0.01f*vec2(1,1)*waveHeight(uv,-5.6f,120.f,0.3f);
    }
    vec3 pos_3d = vec3(position.x, height, -position.y);
    gl_Position = MVP* vec4(pos_3d,1.0);
}
