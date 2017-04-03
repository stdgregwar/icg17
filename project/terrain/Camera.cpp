#include "Camera.h"
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

Camera::Camera(const vec3 &pos) : mRotation(0,0,0),mSSpeed(2), mPosition(pos), mLSpeed(0)
{

}

void Camera::setBaseSpeed(float speed) {
    mSSpeed = speed;
}

void Camera::update(float delta_s) {
    vec3& rot = mRotation;
    vec3 look = {cos(rot.y)*cos(rot.x),
                 cos(rot.y)*sin(rot.x),
                 sin(rot.y)};

    vec3 up = vec3(0,0,1);
    vec3 side = cross(look,up);
    vec3 wspeed = look*mLSpeed.x + side*mLSpeed.y;

    mPosition += wspeed*mSSpeed*delta_s;
    mView = lookAt(mPosition,mPosition+look,up);
}

void Camera::rotate(glm::vec2 delta) {
    mRotation.x += delta.x;
    static float rmax = M_PI/2-0.1;
    static float rmin = -M_PI/2+0.1;
    mRotation.y = min(rmax,max(rmin,delta.y+mRotation.y));
}

void Camera::speed(glm::vec2 delta) {
    mLSpeed.x += delta.x;
    mLSpeed.y += delta.y;
}

void Camera::onKey(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if(action == GLFW_PRESS) {
        switch(key) {
        case GLFW_KEY_W:
            speed({1,0});
            break;
        }
    } else {
        switch(key) {
        case GLFW_KEY_W:
            speed({-1,0});
            break;
        }
    }
}

void Camera::onMouse(GLFWwindow* window, double xpos, double ypos) {
    static float lx = 0,ly =0;
    float xrel = xpos-lx;
    lx = xpos;
    float yrel = ypos-ly;
    ly = ypos;

    mRotation+=vec3{-xrel,yrel,0}*0.01f;
}

const glm::mat4& Camera::view() const {
    return mView;
}
