#pragma once
#include <glm/gtc/type_ptr.hpp>
#include "icg_helper.h"
#include "../Material.h"

class Particle {
public:
    Particle();
    GLuint init();
    void cleanup();
    void draw(const glm::mat4 &view, const glm::mat4 &projection);


    GLfloat Life;
    glm::vec3 Position, Velocity;
    glm::vec4 Color;
    //Material mMaterial; // Material containing the particles texture
private:
};
