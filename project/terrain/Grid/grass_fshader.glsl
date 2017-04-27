#version 330

uniform usampler3D stipple;

in vData {
    vec2 uv;
    float alpha;
} vertex;

out vec3 color;

float sdoor(vec2 sc, float alpha) {
    int al = int(clamp(vertex.alpha*5,0,4));
    int x = int(sc.x) % 4;
    int y = int(sc.y) % 4;
    return float(texelFetch(stipple,ivec3(x,y,al),0).r);
}

void main(void)
{
    //if(sdoor(gl_FragCoord.xy,vertex.alpha)) discard;
    color = vec3(0,1,vertex.alpha);
    color = vec3(sdoor(gl_FragCoord.xy,0.3)*255);
}
