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
    setupCycle();
}

void Light::setupCycle() {
    //Setup orientation
    {
        BezierBuilder<glm::vec3> b;
        b
                (-3,-3,-1)
                (-3,3,-3)
                (3,3,-1)
                (3,3,-0.5)
                (-3,-3,-3);
        mDirCycle = b.build();
    }
    {
        //Setup color
        BezierBuilder<glm::vec3> b;
        b
                (182.f/255,126.f/255,91.f/255)
                (192.f/255,191.f/255,173.f/255)
                (182.f/255,126.f/255,91.f/255)
                (0.1,0.05,0.025)
                (0.1,0.1,0.2);
        mColCycle = b.build();
    }
    {
        //Setup ambient
        BezierBuilder<glm::vec3> b;
        b
                (0.2,0.3,0.3)
                (0.2,0.3,0.3)
                (0.2,0.3,0.3)
                (0.1,0.1,0.1)
                (0.1,0.1,0.1);
        mAmbientCycle = b.build();
    }
    {
        //Setup density
        BezierBuilder<float> b;
        b
                (1)
                (2)
                (1)
                (0.5)
                (0.5);
        mDenCycle = b.build();
    }
}

void Light::update(float delta_s) {
    mTime += 0.05*delta_s;
    setDirection(mDirCycle.curveAtTime(mTime));
    mColor = mColCycle.curveAtTime(mTime);
    mAmbient = mAmbientCycle.curveAtTime(mTime);
    mDensity = mDenCycle.curveAtTime(mTime);
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

}

bool Light::inFrustum(const glm::vec2& pos, const float &chunkSize, size_t i) const {
    glm::vec4 mFrustum[6];
    glm::mat4 VP = mCascades[i].LVP;
    for(int i = 0; i < 3; i++) {
        mFrustum[i].x = VP[0][3] + VP[0][i];
        mFrustum[i].y = VP[1][3] + VP[1][i];
        mFrustum[i].z = VP[2][3] + VP[2][i];
        mFrustum[i].w = VP[3][3] + VP[3][i];

        mFrustum[i+1].x = VP[0][3] - VP[0][i];
        mFrustum[i+1].y = VP[1][3] - VP[1][i];
        mFrustum[i+1].z = VP[2][3] - VP[2][i];
        mFrustum[i+1].w = VP[3][3] - VP[3][i];
        mFrustum[i] = glm::normalize(mFrustum[i]);
        mFrustum[i+1] = glm::normalize(mFrustum[i+1]);
    }

    glm::vec3 mins = glm::vec3(pos,-4096);
    glm::vec3 maxs = mins + glm::vec3(chunkSize,chunkSize,8192);
    glm::vec3 vmin, vmax;

    for(int i =0; i < 6; i++) {
        const float w = mFrustum[i].w;
        const glm::vec3 normal = vec3(mFrustum[i]);

        // X axis
        if(mFrustum[i].x > 0) {
            vmin.x = mins.x;
            vmax.x = maxs.x;
        } else {
            vmin.x = maxs.x;
            vmax.x = mins.x;
        }
        // Y axis
        if(mFrustum[i].y > 0) {
            vmin.y = mins.y;
            vmax.y = maxs.y;
        } else {
            vmin.y = maxs.y;
            vmax.y = mins.y;
        }
        // Z axis
        if(mFrustum[i].z > 0) {
            vmin.z = mins.z;
            vmax.z = maxs.z;
        } else {
            vmin.z = maxs.z;
            vmax.z = mins.z;
        }

        if(glm::dot(normal,vmin)   + w > 0){
            return true;
        }
        if(glm::dot(normal,vmax) + w < 0){
            return false;
        }
    }
    return false;
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
    glUniform1f(m.uniformLocation("l_density"),mDensity);
    glUniform1f(m.uniformLocation("l_daytime"),mTime/5);
}
