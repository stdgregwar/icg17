#pragma once
#include "icg_helper.h"

class ScalarFrameBuffer {

public:
    int width;
    int height;
private:
    GLuint mFramebufferObjectId=0;
    GLuint mColorTextureId = 0;
    GLuint mDepthRenderBufferId=0;
public:
    // warning: overrides viewport!!
    void bind();
    void unbind();
    int init(int imageWidth, int imageHeight, bool useInterpolation = true);
    void cleanup();
};
