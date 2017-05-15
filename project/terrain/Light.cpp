#include "Light.h"
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

Light::Light(const glm::vec3& shadowSize, const glm::vec3& direction, const glm::vec3& color) :
    mSize(shadowSize), mDirection(direction), mColor(color)
{
    mLP = ortho(-mSize.x/2,mSize.x/2,-mSize.y/2,mSize.y/2,-mSize.z/4,3*mSize.z/4);
}

void Light::setDirection(const glm::vec3& dir) {
    mDirection = dir;
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
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
     glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_COMPARE_MODE,GL_COMPARE_REF_TO_TEXTURE);

     mScreenQuad.material().addTexture(GL_TEXTURE_2D,GL_TEXTURE0,mDepthTexture,"buffer_color");

     glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, mDepthTexture, 0);

     glDrawBuffer(GL_NONE); // No color buffer is drawn to.
     glReadBuffer(GL_NONE);

     glBindFramebuffer(GL_FRAMEBUFFER,0);

     // Always check that our framebuffer is ok
     if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
     return false;
     else return true;
}

void Light::draw() const {
    glViewport(0,0,256,256);
    mScreenQuad.draw();
}

void Light::bind(Material& m, const Camera& cam) {
    vec3 pos = cam.pos();
    pos.z = 0;
    //pos.x -= mDirection.x*mSize.x/128;
    //pos.y -= mDirection.y*mSize.y/128;
    mLV = lookAt(pos,pos+mDirection,vec3(0,0,1));
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
    m.addTexture(GL_TEXTURE_2D,GL_TEXTURE9,mDepthTexture,"shadowmap");
    glUniformMatrix4fv(m.uniformLocation("l_VP"), ONE, DONT_TRANSPOSE,glm::value_ptr(mLVP));
    glUniformMatrix4fv(m.uniformLocation("l_V"), ONE, DONT_TRANSPOSE,glm::value_ptr(mLV));
    glUniformMatrix4fv(m.uniformLocation("l_iV"), ONE, DONT_TRANSPOSE,glm::value_ptr(inverse(mLV)));
    glUniformMatrix4fv(m.uniformLocation("l_P"), ONE, DONT_TRANSPOSE,glm::value_ptr(mLV));
    glUniform3f(m.uniformLocation("l_color"),mColor.x,mColor.y,mColor.z);
}
