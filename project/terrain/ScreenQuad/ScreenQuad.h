#pragma once
#include <glm/gtc/type_ptr.hpp>
#include "icg_helper.h"
#include "../Material.h"

class ScreenQuad {

public:
    ScreenQuad(const string& vshader, const string& fshader,float size);
    ScreenQuad();
    void init(const string& vshader, const string& fshader,float size);
    Material& material() {return mMaterial;}
    void cleanup();
    void draw(const glm::mat4 &model = glm::mat4(), float res = 256) const;
private:
    Material mMaterial;
    GLuint mVertexArrayId;        // vertex array object
    GLuint mProgramId;             // GLSL shader program ID
    GLuint mVertexBufferObject;   // memory buffer

    float mSize;

};
