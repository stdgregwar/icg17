#ifndef LIGHT_H
#define LIGHT_H

#include "icg_helper.h"
#include "Material.h"
#include "Camera.h"
#include "ScreenQuad/ScreenQuad.h"
#include "Bezier.h"

#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <array>


class Light
{
public:
    struct Cas {
        glm::mat4 LVP;
        glm::mat4 LV;
        glm::mat4 LP;
        GLuint depth;
        float radius;
        string uname;
    };

    Light(const glm::vec3& shadowSize, const glm::vec3& direction, const glm::vec3& color, const glm::vec3 ambient);
    void update(float delta_s);
    void setupCycle();
    void addTexture(Material& m);
    bool init(size_t texSize = 1024);
    void bind(const Camera& cam, int i);
    void unbind();
    void uniforms(Material& m);
    GLuint depth(int i) const {return mCascades[i].depth;}
    void setDirection(const glm::vec3& dir);
    bool inFrustum(const glm::vec2& pos, const float &chunkSize, size_t i) const ;
    const glm::mat4& proj(int i) const {return mCascades[i].LP;}
    const glm::mat4& view(int i) const {return mCascades[i].LV;}
    void draw() const;
private:
    GLuint mFramebuffer;
    Bezier<glm::vec3> mDirCycle;
    Bezier<glm::vec3> mColCycle;
    Bezier<glm::vec3> mAmbientCycle;
    Bezier<float> mDenCycle;
    std::array<Cas,3> mCascades;
    ScreenQuad mScreenQuad;
    size_t mTexSize;
    glm::vec3 mDirection;
    float mDensity;
    glm::vec3 mColor;
    glm::vec3 mAmbient;
    float mTime = 0;
};

#endif // LIGHT_H
