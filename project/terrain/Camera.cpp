#include "Camera.h"
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <unordered_map>

#ifndef M_PI
#define M_PI 3.1415
#endif

using namespace glm;
using namespace std;

Camera::Camera(const glm::vec3& pos, const glm::vec3& orientation) : mRotation(orientation),mTargetRotation(orientation),mSSpeed(20), mPosition(pos), mLSpeed(0)
{

}

void Camera::setBaseSpeed(float speed) {
    mSSpeed = speed;
}

void Camera::update(float delta_s) {
    mRotation = mRotation + (mTargetRotation - mRotation) * std::min(10.f * delta_s,1.f);

    vec3& rot = mRotation;
    vec3 look = {cos(rot.y)*cos(rot.x),
                 cos(rot.y)*sin(rot.x),
                 sin(rot.y)};

    vec3 up = vec3(0,0,1);
    vec3 side = normalize(cross(look,up));
    vec3 wspeed = look*mLSpeed.x + side*mLSpeed.y;

    mTargetPosition += wspeed*mSSpeed*delta_s;
    mPosition = mPosition + (mTargetPosition - mPosition) * std::min(10.f * delta_s,1.f);
    mView = lookAt(mPosition,mPosition+look,up);
}

void Camera::rotate(glm::vec2 delta) {
    mTargetRotation.x += delta.x;
    static float max = M_PI/2-0.1;
    mTargetRotation.y += delta.y;
    if(abs(mTargetRotation.y) > max) {
        mTargetRotation.y = sign(mTargetRotation.y)*max;
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

const glm::mat4& Camera::projection() const {
    return mProjection;
}


const glm::mat4& Camera::view() const {
    return mView;
}

void Camera::setProjection(const glm::mat4& projection) {
    mProjection = projection;
}

bool Camera::inFrustum(const glm::vec2& pos) const {
    return true;
//    glm::vec4 mFrustum[6];
//    glm::mat4 VP = mProjection*mView;
//    for(int i = 0; i < 6; i++) {
//        mFrustum[i].x = VP[3][0] + VP[0][0];
//        mFrustum[i].y = VP[3][1] + VP[0][1];
//        mFrustum[i].z = VP[3][2] + VP[0][2];
//        mFrustum[i].w = VP[3][3] + VP[0][3];
//        glm::normalize(mFrustum[i]);
//    }

//    for(int i =0; i < 6; i++) {
//        const float w = mFrustum[i].w;
//        const glm::vec3 normal = vec3(mFrustum[i]);
//        for(int x = 0; x <= 1; x++) {
//            for(int y = 0; y<= 1; y++) {
//                for(int z = -1; z <= 1; z += 2) {
//                    glm::vec4 pos4D = VP*glm::vec4(pos+glm::vec2(x,y),1*z,1.0);
//                    glm::vec3 pos3D = glm::vec3(pos4D.x,pos4D.y,pos4D.z)/pos4D.w;
//                    if(glm::dot(pos3D,normal) + w > 0)
//                        return true;
//                }
//            }
//        }
//    }
//    return false;
}


glm::vec2 Camera::wPos() const {
    return {mPosition.x,mPosition.y};
}
