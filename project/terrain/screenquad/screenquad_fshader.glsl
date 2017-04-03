#version 330

in vec2 uv;
uniform vec2 offset;
uniform float screenWidth;
out float value;

#define PI 3.14159265358979323846

#define M_PI 3.14159265358979323846

vec3 permute(vec3 x) { return mod(((x*34.0)+1.0)*x, 289.0); }

float snoise(vec2 v){
  const vec4 C = vec4(0.211324865405187, 0.366025403784439,
           -0.577350269189626, 0.024390243902439);
  vec2 i  = floor(v + dot(v, C.yy) );
  vec2 x0 = v -   i + dot(i, C.xx);
  vec2 i1;
  i1 = (x0.x > x0.y) ? vec2(1.0, 0.0) : vec2(0.0, 1.0);
  vec4 x12 = x0.xyxy + C.xxzz;
  x12.xy -= i1;
  i = mod(i, 289.0);
  vec3 p = permute( permute( i.y + vec3(0.0, i1.y, 1.0 ))
  + i.x + vec3(0.0, i1.x, 1.0 ));
  vec3 m = max(0.5 - vec3(dot(x0,x0), dot(x12.xy,x12.xy),
    dot(x12.zw,x12.zw)), 0.0);
  m = m*m ;
  m = m*m ;
  vec3 x = 2.0 * fract(p * C.www) - 1.0;
  vec3 h = abs(x) - 0.5;
  vec3 ox = floor(x + 0.5);
  vec3 a0 = x - ox;
  m *= 1.79284291400159 - 0.85373472095314 * ( a0*a0 + h*h );
  vec3 g;
  g.x  = a0.x  * x0.x  + h.x  * x0.y;
  g.yz = a0.yz * x12.xz + h.yz * x12.yw;
  return 130.0 * dot(m, g);
}

float noise(vec2 p) {
    return snoise(p);
}

float fbm(vec2 x, int octaves) {
    float v = 0.0;
    float a = 0.5;
    vec2 shift = vec2(100);
    // Rotate to reduce axial bias
    mat2 rot = mat2(cos(0.5), sin(0.5), -sin(0.5), cos(0.50));
    for (int i = 0; i < octaves; ++i) {
        v += a * noise(x);
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
        value *= ((1-abs(noise(point))) + offset) * pow(lacunarity, -H*i);
        point *= lacunarity;
    }
    return value;
}

void main() {
        float scale = 2;
        vec2 p = gl_FragCoord.xy*0.001*scale + offset;

        //value = fbm(p,1)*20+10;
        value = (multifractal(p*0.6,0.1,0.5,6,0.7)-10)*0.8;//*fbm(p*1,5)*0.5;
        value = max(0.1,value);
        value += (value*0.08+1)*fbm(p*1,8)*4;
        value /= scale;
        //value = max(0,value);
        //value = fbm(p,8);
        //value = noise(p);
}
