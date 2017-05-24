#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <GLFW/glfw3.h>
#include "Camera.h"
#include "Bezier.h"

class CameraFreefly: public Camera
{
public:
    CameraFreefly(const glm::vec3& pos, const glm::vec3& orientation = {0,0,0});
    void setBaseSpeed(float speed);
    void update(float delta_s, const Chunk &c) override;
    void rotate(glm::vec2 delta);
    void speed(glm::vec2 delta);
    void onKey(GLFWwindow* window, int key, int scancode, int action, int mods);
    void onMouse(GLFWwindow* window, double xpos, double ypos);
private:
    glm::vec3 inMap(const glm::vec3& pos, const Chunk& c);
    enum Plane
                   {
                           PLANE_BACK,
                           PLANE_FRONT,
                           PLANE_RIGHT,
                           PLANE_LEFT,
                           PLANE_TOP,
                           PLANE_BOTTOM
                   };
    float mSSpeed;
    glm::vec3 mLSpeed;
    glm::vec3 mTargetPosition;
    glm::vec3 mTargetRotation;
};
