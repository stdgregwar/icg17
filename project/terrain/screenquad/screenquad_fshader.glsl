#version 330

in vec2 uv;
uniform vec2 offset;
uniform float screenWidth;
out float value;

#define PI 3.14159265358979323846

#define M_PI 3.14159265358979323846

float rand(vec2 co){return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);}
float rand (vec2 co, float l) {return rand(vec2(rand(co), l));}
float rand (vec2 co, float l, float t) {return rand(vec2(rand(co, l), t));}

float perlin(vec2 p, float dim, float time) {
        vec2 pos = floor(p * dim);
        vec2 posx = pos + vec2(1.0, 0.0);
        vec2 posy = pos + vec2(0.0, 1.0);
        vec2 posxy = pos + vec2(1.0);

        float c = rand(pos, dim, time);
        float cx = rand(posx, dim, time);
        float cy = rand(posy, dim, time);
        float cxy = rand(posxy, dim, time);

        vec2 d = fract(p * dim);
        d = -0.5 * cos(d * M_PI) + 0.5;

        float ccx = mix(c, cx, d.x);
        float cycxy = mix(cy, cxy, d.x);
        float center = mix(ccx, cycxy, d.y);

        return center * 2.0 - 1.0;
}

float noise(vec2 p) {
    return perlin(p,2,0);
}

float fbm(vec2 x, int octaves) {
    float v = 0.0;
    float a = 0.5;
    vec2 shift = vec2(100);
    // Rotate to reduce axial bias
    mat2 rot = mat2(cos(0.5), sin(0.5), -sin(0.5), cos(0.50));
    for (int i = 0; i < octaves; ++i) {
        v += a * (1-abs(noise(x)));
        x = rot * x * 2.0 + shift;
        a *= 0.5;
    }
    return v;
}

float multifractal(vec2 point, float H, float lacunarity, int
                   octaves, float offset)
{
    float value = 1.0;
    for (int i = 0; i < octaves; i++) {
        value *= (1-abs(noise(point)) + offset) * pow(lacunarity, -H*i);
        point *= lacunarity;
    }
    return value;
}

void main() {
    vec2 p = gl_FragCoord.xy*0.001 + offset;
    value = multifractal(p,0.3,0.9,8,0.2)*fbm(p,8);
    //value = fbm(p,8);
    //value = noise(p);
}

