#include "Light.h"
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

Light::Light(const glm::vec3& shadowSize, const glm::vec3& direction, const glm::vec3& color, const vec3 ambient) :
   mDirection(direction), mColor(color), mAmbient(ambient)
{
    glm::vec3 size = shadowSize;
    for(int i = 0; i < mCascades.size(); i++) {
        Cas& c = mCascades[i];
        c.uname = "cascades[" + to_string(i) + "]";
        c.LP = ortho(-size.x/2,size.x/2,-size.y/2,size.y/2,-size.z/2,size.z/2);
        c.radius = std::min(size.x,std::min(size.y,size.z))/2;
        size.x /= 4;
        size.y /= 4;
    }
}

void Light::setDirection(const glm::vec3& dir) {
    mDirection = dir;
}

bool Light::init(size_t texSize) {
    mTexSize = texSize;
    mScreenQuad.init("vbuffercopy.glsl","fbuffercopy.glsl");
    // The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.

    mFramebuffer = 0;
    glGenFramebuffers(1, &mFramebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, mFramebuffer);
    for(Cas& c : mCascades) {
        // Depth texture. Slower than a depth buffer, but you can sample it later in your shader
        glGenTextures(1, &c.depth);
        glBindTexture(GL_TEXTURE_2D, c.depth);
        glTexImage2D(GL_TEXTURE_2D, 0,GL_DEPTH_COMPONENT16, texSize, texSize, 0,GL_DEPTH_COMPONENT, GL_FLOAT, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_COMPARE_MODE,GL_COMPARE_REF_TO_TEXTURE);

        mScreenQuad.material().addTexture(GL_TEXTURE_2D,GL_TEXTURE0,c.depth,c.uname);

        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, c.depth, 0);

        glDrawBuffer(GL_NONE); // No color buffer is drawn to.
        glReadBuffer(GL_NONE);
        // Always check that our framebuffer is ok

    }
    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D,mCascades[1].depth,0);
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        throw std::runtime_error("Framebuffer error in light");
    }
    glBindFramebuffer(GL_FRAMEBUFFER,0);
    return true;
}

void Light::draw() const {
    glViewport(0,0,256,256);
    mScreenQuad.draw();
}

void Light::bind(const Camera& cam, int i) {
    Cas& c = mCascades[i];
    vec3 pos = cam.pos();
    vec3 fo = cam.forward();
    pos += fo * c.radius;
    //pos.z = 0;
    c.LV = lookAt(pos,pos+mDirection,vec3(0,0,1));
    c.LVP = c.LP*c.LV;

    glViewport(0, 0, mTexSize, mTexSize);
    glBindFramebuffer(GL_FRAMEBUFFER, mFramebuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, c.depth, 0);
    glClear(GL_DEPTH_BUFFER_BIT);
}

void Light::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Light::addTexture(Material& m) {
    for(int i = 0; i < mCascades.size(); i++) {
        Cas& c = mCascades[i];
        m.addTexture(GL_TEXTURE_2D,GL_TEXTURE9+i,c.depth,c.uname);
    }
}

void Light::uniforms(Material& m) {
    //
    for(int i = 0; i < mCascades.size(); i++) {
        Cas& c = mCascades[i];
        string is = "[" + to_string(i)+ "]";
        glUniformMatrix4fv(m.uniformLocation("l_VP"+is), ONE, DONT_TRANSPOSE,glm::value_ptr(c.LVP));
        glUniformMatrix4fv(m.uniformLocation("l_V"+is), ONE, DONT_TRANSPOSE,glm::value_ptr(c.LV));
        glUniformMatrix4fv(m.uniformLocation("l_iV"+is), ONE, DONT_TRANSPOSE,glm::value_ptr(inverse(c.LV)));
        glUniformMatrix4fv(m.uniformLocation("l_P"+is), ONE, DONT_TRANSPOSE,glm::value_ptr(c.LP));
    }
    glUniform3f(m.uniformLocation("l_color"),mColor.x,mColor.y,mColor.z);
    glUniform3f(m.uniformLocation("l_ambient"),mAmbient.x,mAmbient.y,mAmbient.z);
}
