#pragma once
#include <glm/gtc/type_ptr.hpp>
#include "icg_helper.h"
#include "../Material.h"

class Particles {
public:
    Particles();
    GLuint init();
    void cleanup();
    void draw(const glm::mat4 &view, const glm::mat4 &projection);
private:
    Material mMaterial; // Material containing the particles texture
    glm::vec2 Position, Velocity;
    glm::vec4 Color;
    GLfloat Life;
};
