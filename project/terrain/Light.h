#ifndef LIGHT_H
#define LIGHT_H

#include "icg_helper.h"
#include "Material.h"

#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Light
{
public:
    Light();
    bool init(size_t texSize = 1024);
    void bind(Material& m);
    void unbind();

    void uniforms(Material& m);
private:
    GLuint mFrameBuffer;
    glm::vec3 mDirection;
    glm::vec3 mColor;
    float mEnergy;
};

#endif // LIGHT_H
