#pragma once
#include "icg_helper.h"

class ScalarFrameBuffer {

public:
    int width;
    int height;
private:
    GLuint mFramebufferObjectId;
    GLuint mColorTextureId = 0;
    GLuint mDepthRenderBufferId;
public:
    // warning: overrides viewport!!
    void attach(GLuint texture);
    void bind();
    void unbind();
    int init(int imageWidth, int imageHeight, bool useInterpolation = true);
    void cleanup();
};
