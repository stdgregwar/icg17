#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <GLFW/glfw3.h>

class Camera
{
public:
    Camera(const glm::vec3& pos);
    void setBaseSpeed(float speed);
    void update(float delta_s);
    void rotate(glm::vec2 delta);
    void speed(glm::vec2 delta);
    void onKey(GLFWwindow* window, int key, int scancode, int action, int mods);
    void onMouse(GLFWwindow* window, double xpos, double ypos);
    const glm::mat4& view() const;
private:
    float mSSpeed;
    glm::vec3 mLSpeed;
    glm::vec3 mPosition;
    glm::vec3 mRotation;
    glm::mat4 mView;
};
