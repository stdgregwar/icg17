#pragma once
#include "icg_helper.h"

class FrameBuffer {

public:
    int width;
    int height;
private:
    GLuint mFramebufferObjectId=0;
    GLuint mColorTextureId = 0;
    GLuint mNormalTextureId = 0;
    GLuint mDepthTextureId=0;
public:
    // warning: overrides viewport!!
    void bind();
    void blit(GLuint fb);
    void unbind();
    std::pair<int,int> init(int imageWidth, int imageHeight, bool useInterpolation = true);
    GLuint normal() const {return mNormalTextureId;}
    GLuint diffuse() const {return mColorTextureId;}
    GLuint depth() const {return mDepthTextureId;}

    void cleanup();
    ~FrameBuffer();
};
