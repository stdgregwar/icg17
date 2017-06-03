#version 330

uniform sampler2D buffer_color;
uniform sampler2D buffer_depth;

uniform mat4 V;
uniform mat4 P;

in vec2 uv;
out vec4 color;

const float exposure = 1;
const float decay = 0.2;
const float density = 0.9;
const float weight = 0.1;
const vec2 lightPositionOnScreen = vec2(1,1);
const int NUM_SAMPLES = 100;

void main()
{
    vec2 lightPositionOnScreen = (P*V*vec4(0,0,0,1)).xy*0.5+vec2(0.5);
    vec2 deltaTextCoord = vec2(uv - lightPositionOnScreen);
    vec2 textCoo = uv;
    deltaTextCoord *= 1.0 /  float(NUM_SAMPLES) * density;
    float illuminationDecay = 1.0;


    for(int i=0; i < NUM_SAMPLES ; i++)
    {
	     textCoo -= deltaTextCoord;
	     vec4 sample = texture2D(buffer_color, textCoo);

	     sample *= illuminationDecay * weight;

	     color += sample;

	     illuminationDecay *= decay;
     }
     color *= exposure;
     //color = vec4(deltaTextCoord,0,1);
}
