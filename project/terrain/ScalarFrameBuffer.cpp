#include "ScalarFrameBuffer.h"

// warning: overrides viewport!!
void ScalarFrameBuffer::bind() {
    glViewport(0, 0, width, height);
    glBindFramebuffer(GL_FRAMEBUFFER, mFramebufferObjectId);
    const GLenum buffers[] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1 /*length of buffers[]*/, buffers);
}

void ScalarFrameBuffer::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

int ScalarFrameBuffer::init(int imageWidth, int imageHeight, bool useInterpolation) {
    width = imageWidth;
    height = imageHeight;
    if(mColorTextureId)
        cleanup();
    // create color attachment
    {
        glGenTextures(1, &mColorTextureId);
        glBindTexture(GL_TEXTURE_2D, mColorTextureId);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        if(useInterpolation){
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        } else {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        }

        // create texture for the color attachment
        // see Table.2 on
        // khronos.org/opengles/sdk/docs/man3/docbook4/xhtml/glTexImage2D.xml
        glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, width, height, 0,
                     GL_RED, GL_FLOAT, NULL);
        // how to load from buffer
    }

    // create render buffer (for depth channel)
    {
        glGenRenderbuffers(1, &mDepthRenderBufferId);
        glBindRenderbuffer(GL_RENDERBUFFER, mDepthRenderBufferId);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, width, height);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
    }

    // tie it all together
    {
        glGenFramebuffers(1, &mFramebufferObjectId);
        glBindFramebuffer(GL_FRAMEBUFFER, mFramebufferObjectId);
        glFramebufferTexture2D(GL_FRAMEBUFFER,
                               GL_COLOR_ATTACHMENT0 /*location = 0*/,
                               GL_TEXTURE_2D, mColorTextureId,
                               0 /*level*/);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                                                 GL_RENDERBUFFER, mDepthRenderBufferId);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) !=
                GL_FRAMEBUFFER_COMPLETE) {
            throw std::runtime_error("ERROR: Framebuffer not OK :(");
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0); // avoid pollution
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    return mColorTextureId;
}

void ScalarFrameBuffer::cleanup() {
    //glDeleteTextures(1, &mColorTextureId);
    glDeleteRenderbuffers(1, &mDepthRenderBufferId);
    glBindFramebuffer(GL_FRAMEBUFFER, 0 /*UNBIND*/);
    glDeleteFramebuffers(1, &mFramebufferObjectId);
}

ScalarFrameBuffer::~ScalarFrameBuffer() {
    cleanup();
}
