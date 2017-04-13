#pragma once
#include <glm/gtc/type_ptr.hpp>
#include "icg_helper.h"
#include "../Material.h"

class Skybox {
public:
    Skybox();
    GLuint init();
    void cleanup();
    void draw(const glm::mat4 &view, const glm::mat4 &projection);
private:
    Material mMaterial; // Material containing the cube map texture
    GLuint skyboxVAO;  // Vertex array
    GLuint skyboxVBO;  // Buffer object
};
