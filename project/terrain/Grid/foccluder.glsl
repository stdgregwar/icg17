#version 330

#pragma include"grass_foccluder.glsl"


void main(){
    // Not really needed, OpenGL does it anyway
    //fragmentdepth = gl_FragCoord.z;
    gl_FragDepth = gl_FragCoord.z;
}
