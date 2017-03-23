#version 330

in vec2 uv;

out vec3 color;

uniform sampler2D tex;
uniform float tex_width;
uniform float tex_height;

const float[9] Gx = float[](-1,0,1,-2,0,2,-1,0,1);
const float[9] Gy = float[](-1,0,1,-2,0,2,-1,0,1);
const float[121] N = float[](0.0001, 0.0002, 0.0006, 0.0011, 0.0016, 0.0018, 0.0016, 0.0011, 0.0006, 0.0002, 0.0001, 0.0002, 0.0007, 0.0018, 0.0033, 0.0048, 0.0054, 0.0048, 0.0033, 0.0018, 0.0007, 0.0002, 0.0006, 0.0018, 0.0042, 0.0079, 0.0115, 0.0131, 0.0115, 0.0079, 0.0042, 0.0018, 0.0006, 0.0011, 0.0033, 0.0079, 0.0148, 0.0215, 0.0244, 0.0215, 0.0148, 0.0079, 0.0033, 0.0011, 0.0016, 0.0048, 0.0115, 0.0215, 0.0313, 0.0355, 0.0313, 0.0215, 0.0115, 0.0048, 0.0016, 0.0018, 0.0054, 0.0131, 0.0244, 0.0355, 0.0402, 0.0355, 0.0244, 0.0131, 0.0054, 0.0018, 0.0016, 0.0048, 0.0115, 0.0215, 0.0313, 0.0355, 0.0313, 0.0215, 0.0115, 0.0048, 0.0016, 0.0011, 0.0033, 0.0079, 0.0148, 0.0215, 0.0244, 0.0215, 0.0148, 0.0079, 0.0033, 0.0011, 0.0006, 0.0018, 0.0042, 0.0079, 0.0115, 0.0131, 0.0115, 0.0079, 0.0042, 0.0018, 0.0006, 0.0002, 0.0007, 0.0018, 0.0033, 0.0048, 0.0054, 0.0048, 0.0033, 0.0018, 0.0007, 0.0002, 0.0001, 0.0002, 0.0006, 0.0011, 0.0016, 0.0018, 0.0016, 0.0011, 0.0006, 0.0002, 0.0001);

float rgb_2_luma(vec3 c) {
    return 0.3*c[0] + 0.59*c[1] + 0.11*c[2];
}

void main() {
//    color = texture(tex,uv).rgb;
//    float conv1 = 0.0;
//    float conv2 = 0.0;
//    conv1 += -1 * rgb_2_luma( textureOffset(tex,uv,ivec2(-1,-1)).rgb);
//    conv1 += 1 * rgb_2_luma( textureOffset(tex,uv,ivec2(-1,1)).rgb);

//    conv1 += -2 * rgb_2_luma( textureOffset(tex,uv,ivec2(0,-1)).rgb);
//    conv1 += 2 * rgb_2_luma( textureOffset(tex,uv,ivec2(0,1)).rgb);

//    conv1 += -1 * rgb_2_luma( textureOffset(tex,uv,ivec2(1,-1)).rgb);
//    conv1 += 1 * rgb_2_luma( textureOffset(tex,uv,ivec2(1,1)).rgb);

//    conv2 += -1 * rgb_2_luma( textureOffset(tex,uv,ivec2(-1,-1)).rgb);
//    conv2 += -2 * rgb_2_luma( textureOffset(tex,uv,ivec2(-1,0)).rgb);
//    conv2 += -1 * rgb_2_luma( textureOffset(tex,uv,ivec2(-1,1)).rgb);

//    conv2 += 1 * rgb_2_luma( textureOffset(tex,uv,ivec2(1,-1)).rgb);
//    conv2 += 2 * rgb_2_luma( textureOffset(tex,uv,ivec2(1,0)).rgb);
//    conv2 += 1 * rgb_2_luma( textureOffset(tex,uv,ivec2(1,1)).rgb);

//    color = vec3(1.0 - sqrt(conv1*conv1+conv2*conv2));


//    for(int i = 0; i < 9; ++i) {
//        conv1 += Gx[i] * rgb_2_luma( textureOffset(tex, uv, ivec2(i%3-1,i/3-1)).rgb);
//        conv2 += Gy[i] * rgb_2_luma( textureOffset(tex, uv, ivec2(i%3-1,i/3-1)).rgb);
//    }

    float tot_weight = 0;
    vec3 tot_image = vec3(0.0);
    for(int i = 0; i < 11; i++) {
        for(int j = 0; j < 11; j++) {
            vec3 image = texture(tex, uv+vec2(i/tex_width,j/tex_height)).rgb;
            float weight = N[i*11+j];
            tot_image += weight * image;
            tot_weight += weight;
        }
    }
    color = tot_image/tot_weight;
}

