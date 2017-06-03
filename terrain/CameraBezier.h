#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <GLFW/glfw3.h>
#include "Camera.h"
#include "Bezier.h"
#include "VecAndDiff.h"

class CameraBezier: public Camera
{
public:
    CameraBezier(const glm::vec3& pos, const glm::vec3& orientation, const Bezier<VecAndDiff>& bezier);
    CameraBezier(const Bezier<VecAndDiff>& bezier);
    CameraBezier(const glm::vec3& pos, const glm::vec3& orientation, const vector< vector<VecAndDiff> > &pathControlPoints);
    void update(float delta_s, const Chunk& c);
    void rotate(glm::vec2 delta);
    void speed(glm::vec2 delta);
    void onKey(GLFWwindow* window, int key, int scancode, int action, int mods);
    void onMouse(GLFWwindow* window, double xpos, double ypos);
    void tGravity() {}
    void updateBezier(const Bezier<VecAndDiff>& bezier) {mBezierPath = bezier;}
private:
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
    Bezier<VecAndDiff> mBezierPath;
    Bezier<float> mBezierSpeed;
    glm::vec3 mLSpeed;
    glm::vec3 mTargetPosition;
    glm::vec3 mTargetRotation;
};
