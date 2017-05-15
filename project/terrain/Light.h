#ifndef LIGHT_H
#define LIGHT_H

#include "icg_helper.h"
#include "Material.h"
#include "Camera.h"
#include "ScreenQuad/ScreenQuad.h"

#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Light
{
public:
    Light(const glm::vec3& shadowSize, const glm::vec3& direction, const glm::vec3& color);
    bool init(size_t texSize = 1024);
    void bind(Material& m, const Camera& cam);
    void unbind();
    void uniforms(Material& m);
    GLuint depth() const {return mDepthTexture;}
    void setDirection(const glm::vec3& dir);
    const glm::mat4& proj() const {return mLP;}
    const glm::mat4& view() const {return mLV;}
    void draw() const;
private:
    ScreenQuad mScreenQuad;
    GLuint mFrameBuffer;
    GLuint mDepthTexture;
    size_t mTexSize;
    glm::vec3 mDirection;
    glm::vec3 mColor;
    glm::vec3 mSize;
    glm::mat4 mLVP;
    glm::mat4 mLV;
    glm::mat4 mLP;
};

#endif // LIGHT_H
