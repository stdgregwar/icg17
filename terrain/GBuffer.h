#pragma once
#include "icg_helper.h"

class GBuffer {

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
    void blit(const GBuffer &other);
    GLuint id() const {return mFramebufferObjectId;}
    void unbind();
    std::pair<int,int> init(int imageWidth, int imageHeight, bool useInterpolation = true);
    GLuint diffuse() const {return mColorTextureId;}
    GLuint depth() const {return mDepthTextureId;}
    GLuint normal() const {return mNormalTextureId;}

    void cleanup();
    ~GBuffer();
};
