#pragma once
#include <glm/gtc/type_ptr.hpp>
#include "icg_helper.h"

class NoiseGen {

public:
    NoiseGen();
    NoiseGen(float size);
    void init(float size);
    void cleanup();
    void draw(const glm::mat4 &model, float res = 256) const;
private:
    GLuint mVertexArrayId;        // vertex array object
    GLuint mProgramId;             // GLSL shader program ID
    GLuint mVertexBufferObject;   // memory buffer

    float mSize;

};
