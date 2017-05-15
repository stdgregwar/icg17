const mat4 thresholdMatrix = mat4(
            vec4(1.f,13.f,4.f,16.f),
            vec4(9.f,5.f,12.f,8.f),
            vec4(3.f,15.f,2.f,14.f),
            vec4(11.f,7.f,10.f,6.f)
        ) / 17.f;

bool sdoor(vec2 spos, float alpha) {
    int x = int(gl_FragCoord.x);
    int y = int(gl_FragCoord.y);
    return alpha - thresholdMatrix[x % 4][y % 4] < 0;
}
