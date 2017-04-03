#include "Camera.h"
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <unordered_map>

using namespace glm;
using namespace std;

Camera::Camera(const vec3 &pos, const vec3 &orientation) : mRotation(orientation),mSSpeed(20), mPosition(pos), mLSpeed(0)
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
    static float max = M_PI/2-0.1;
    mRotation.y += delta.y;
    if(abs(mRotation.y) > max) {
        mRotation.y = sign(mRotation.y)*max;
    }
}

void Camera::speed(glm::vec2 delta) {
    mLSpeed.x += delta.x;
    mLSpeed.y += delta.y;
}


void Camera::onKey(GLFWwindow* window, int key, int scancode, int action, int mods) {
    unordered_map<int,vec2> pressed{
        {GLFW_KEY_W,{1,0}},
        {GLFW_KEY_S,{-1,0}},
        {GLFW_KEY_A,{0,-1}},
        {GLFW_KEY_D,{0,1}}
    };
    if(pressed.count(key)) {
        if(action == GLFW_PRESS) {
            speed(pressed.at(key));
        } else if (action == GLFW_RELEASE) {
            speed(-pressed.at(key));
        }
    }
    if(key == GLFW_KEY_LEFT_SHIFT) {
        if(action == GLFW_PRESS) {
            mSSpeed *= 4;
        } else if(action == GLFW_RELEASE) {
            mSSpeed /= 4;
        }
    }
}

void Camera::onMouse(GLFWwindow* window, double xpos, double ypos) {
    static float lx = 0,ly =0;
    float xrel = xpos-lx;
    lx = xpos;
    float yrel = ypos-ly;
    ly = ypos;

    rotate(vec2{-xrel,-yrel}*0.001f);
}

const glm::mat4& Camera::view() const {
    return mView;
}

glm::vec2 Camera::wPos() const {
    return {mPosition.x,mPosition.y};
}
