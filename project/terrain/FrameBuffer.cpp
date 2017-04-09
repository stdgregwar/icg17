#include "FrameBuffer.h"

// warning: overrides viewport!!
void FrameBuffer::bind() {
    glViewport(0, 0, width, height);
    glBindFramebuffer(GL_FRAMEBUFFER, mFramebufferObjectId);
    const GLenum buffers[] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1 /*length of buffers[]*/, buffers);
}

void FrameBuffer::blit(GLuint fb) {

     glBindFramebuffer (GL_DRAW_FRAMEBUFFER, 0);
     glDrawBuffer      (fb);              /* Use backbuffer as color dst.         */

     /* Read from your FBO */
     glBindFramebuffer (GL_READ_FRAMEBUFFER, mFramebufferObjectId);
     glReadBuffer      (GL_COLOR_ATTACHMENT0); /* Use Color Attachment 0 as color src. */

     /* Copy the color and depth buffer from your FBO to the default framebuffer       */
     glBlitFramebuffer (0,0, width,height,
                        0,0, width,height,
                        GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT,
                        GL_NEAREST);
}

void FrameBuffer::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

std::pair<int, int> FrameBuffer::init(int imageWidth, int imageHeight, bool useInterpolation) {
    width = imageWidth;
    height = imageHeight;
    if(mColorTextureId || mDepthTextureId)
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
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0,
                     GL_RGBA, GL_FLOAT, NULL);
        // how to load from buffer
    }

    // create render buffer (for depth channel)
    {
        glGenTextures(1, &mDepthTextureId);
        glBindTexture(GL_TEXTURE_2D, mDepthTextureId);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        if(useInterpolation) {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        } else {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        }

        // create texture for the color attachment
        // see Table.2 on
        // khronos.org/opengles/sdk/docs/man3/docbook4/xhtml/glTexImage2D.xml
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0,
                     GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
    }

    // tie it all together
    {
        glGenFramebuffers(1, &mFramebufferObjectId);
        glBindFramebuffer(GL_FRAMEBUFFER, mFramebufferObjectId);
        glFramebufferTexture2D(GL_FRAMEBUFFER,
                               GL_COLOR_ATTACHMENT0 /*location = 0*/,
                               GL_TEXTURE_2D, mColorTextureId,
                               0 /*level*/);
        glFramebufferTexture2D(GL_FRAMEBUFFER,
                               GL_DEPTH_ATTACHMENT /*location = 0*/,
                               GL_TEXTURE_2D, mDepthTextureId,
                               0 /*level*/);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) !=
                GL_FRAMEBUFFER_COMPLETE) {
            throw std::runtime_error("ERROR: Framebuffer not OK :(");
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0); // avoid pollution
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    return std::pair<int,int>(mColorTextureId,mDepthTextureId);
}

void FrameBuffer::cleanup() {
    glDeleteTextures(1, &mColorTextureId);
    glDeleteTextures(1, &mDepthTextureId);
    //glDeleteRenderbuffers(1, &mDepthRenderBufferId);
    glBindFramebuffer(GL_FRAMEBUFFER, 0 /*UNBIND*/);
    glDeleteFramebuffers(1, &mFramebufferObjectId);
}

FrameBuffer::~FrameBuffer() {
    cleanup();
}
