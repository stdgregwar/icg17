#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <GLFW/glfw3.h>

class Chunk;

class Camera
{
public:
    Camera(const glm::vec3& pos, const glm::vec3& orientation = {0,0,0});
    virtual void update(float delta_s, const Chunk& c) = 0;
    virtual void rotate(glm::vec2 delta) = 0;
    virtual void speed(glm::vec2 delta) = 0;
    virtual void onKey(GLFWwindow* window, int key, int scancode, int action, int mods) = 0;
    virtual void onMouse(GLFWwindow* window, double xpos, double ypos) = 0;
    const glm::mat4& view() const;
    glm::vec3 forward() const;
    const glm::mat4& projection() const;
    void setProjection(const glm::mat4& projection);
    bool inFrustum(const glm::vec2& pos, const float &chunkSize) const;
    glm::vec2 wPos() const;
    const glm::vec3 pos() const {return mPosition;}
protected:
    glm::vec3 mPosition;
    glm::vec3 mRotation;
    glm::mat4 mView;
    glm::mat4 mProjection;
};
