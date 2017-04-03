#pragma once
#include "icg_helper.h"

class NoiseGen {

private:
    GLuint mVertexArrayId;        // vertex array object
    GLuint mProgramId;             // GLSL shader program ID
    GLuint mVertexBufferObject;   // memory buffer

    float mScreenquadWidth;
    float mScreenquadHeight;

public:
    void init(float screenquadWidth, float screenquadHeight);
    void cleanup();
    void updateSize(int screenquadWidth, int screenquadHeight);
    void draw(const glm::vec2& offset);
};
