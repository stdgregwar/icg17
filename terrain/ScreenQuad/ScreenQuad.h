#pragma once
#include <glm/gtc/type_ptr.hpp>
#include "icg_helper.h"
#include "../Material.h"

using namespace glm;

class ScreenQuad {
public:
    ScreenQuad(const string& vshader, const string& fshader,float size);
    ScreenQuad();
    void init(const string& vshader, const string& fshader,float size = 256);
    Material& material() {return mMaterial;}
    void cleanup();
    void draw(const mat4 &model = mat4(), float res = 256) const;
    void draw(const mat4 &view, const mat4& projection) const;
private:
    Material mMaterial;
    GLuint mVertexArrayId;        // vertex array object
    GLuint mProgramId;             // GLSL shader program ID
    GLuint mVertexBufferObject;   // memory buffer

    float mSize;

};
