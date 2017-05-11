#include "Light.h"
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

Light::Light(const glm::vec3& shadowSize, const glm::vec3& direction, const glm::vec3& color) :
    mSize(shadowSize), mDirection(direction), mColor(color)
{
    mLP = ortho(-mSize.x/2,mSize.x/2,-mSize.y/2,mSize.y/2,-mSize.z/2,mSize.z/2);
}


bool Light::init(size_t texSize) {
    mTexSize = texSize;
    mScreenQuad.init("vbuffercopy.glsl","fbuffercopy.glsl");
    // The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
     mFrameBuffer = 0;
     glGenFramebuffers(1, &mFrameBuffer);
     glBindFramebuffer(GL_FRAMEBUFFER, mFrameBuffer);

     // Depth texture. Slower than a depth buffer, but you can sample it later in your shader
     glGenTextures(1, &mDepthTexture);
     glBindTexture(GL_TEXTURE_2D, mDepthTexture);
     glTexImage2D(GL_TEXTURE_2D, 0,GL_DEPTH_COMPONENT16, texSize, texSize, 0,GL_DEPTH_COMPONENT, GL_FLOAT, 0);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

     mScreenQuad.material().addTexture(GL_TEXTURE_2D,GL_TEXTURE0,mDepthTexture,"buffer_color");

     glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, mDepthTexture, 0);

     glDrawBuffer(GL_NONE); // No color buffer is drawn to.
     glReadBuffer(GL_NONE);

     // Always check that our framebuffer is ok
     if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
     return false;
     else return true;
}

void Light::draw() const {
    mScreenQuad.draw();
}

void Light::bind(Material& m, const Camera& cam) {
    mLV = lookAt(cam.pos(),cam.pos()+mDirection,vec3(0,0,1));
    mLVP = mLP*mLV;
    //glUniformMatrix4fv(m.uniformLocation("l_MVP"), ONE, DONT_TRANSPOSE,glm::value_ptr(mLMVP));
    glViewport(0, 0, mTexSize, mTexSize);
    glBindFramebuffer(GL_FRAMEBUFFER, mFrameBuffer);
    glClear(GL_DEPTH_BUFFER_BIT);
}

void Light::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Light::uniforms(Material& m) {
    glUniformMatrix4fv(m.uniformLocation("l_VP"), ONE, DONT_TRANSPOSE,glm::value_ptr(mLVP));
    glUniform3f(m.uniformLocation("l_color"),mColor.x,mColor.y,mColor.z);
}
