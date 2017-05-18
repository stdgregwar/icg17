#pragma once
#include "icg_helper.h"

class FrameBuffer {

public:
    int width;
    int height;
private:
    GLuint mFramebufferObjectId=0;
    GLuint mColorTextureId = 0;
    GLuint mDepthTextureId=0;
public:
    // warning: overrides viewport!!
    void bind();
    void blit(GLuint fb);
    void blit(const FrameBuffer& other);
    GLuint id() const {return mFramebufferObjectId;}
    void unbind();
    std::pair<int,int> init(int imageWidth, int imageHeight, bool useInterpolation = true);
    GLuint diffuse() const {return mColorTextureId;}
    GLuint depth() const {return mDepthTextureId;}

    void cleanup();
    ~FrameBuffer();
};
