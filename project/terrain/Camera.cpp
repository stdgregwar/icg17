#include "Camera.h"
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <unordered_map>

#ifndef M_PI
#define M_PI 3.1415
#endif

using namespace glm;
using namespace std;

Camera::Camera(const vec3 &pos, const vec3 &orientation) : mRotation(orientation),mTargetRotation(orientation),mSSpeed(20), mPosition(pos), mTargetPosition(pos), mLSpeed(0)
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

bool Camera::inFrustum(const glm::vec2& pos, const float &chunkSize) const {
    glm::vec4 mFrustum[6];
    glm::mat4 VP = mProjection*mView;
    for(int i = 0; i < 3; i++) {
        mFrustum[i].x = VP[0][3] + VP[0][i];
        mFrustum[i].y = VP[1][3] + VP[1][i];
        mFrustum[i].z = VP[2][3] + VP[2][i];
        mFrustum[i].w = VP[3][3] + VP[3][i];

        mFrustum[i+1].x = VP[0][3] - VP[0][i];
        mFrustum[i+1].y = VP[1][3] - VP[1][i];
        mFrustum[i+1].z = VP[2][3] - VP[2][i];
        mFrustum[i+1].w = VP[3][3] - VP[3][i];
        mFrustum[i] = glm::normalize(mFrustum[i]);
        mFrustum[i+1] = glm::normalize(mFrustum[i+1]);
    }

    glm::vec3 mins = glm::vec3(pos,-4096);
    glm::vec3 maxs = mins + glm::vec3(chunkSize,chunkSize,8192);
    glm::vec3 vmin, vmax;

    for(int i =0; i < 6; i++) {
        const float w = mFrustum[i].w;
        const glm::vec3 normal = vec3(mFrustum[i]);

        // X axis
        if(mFrustum[i].x > 0) {
            vmin.x = mins.x;
            vmax.x = maxs.x;
        } else {
            vmin.x = maxs.x;
            vmax.x = mins.x;
        }
        // Y axis
        if(mFrustum[i].y > 0) {
            vmin.y = mins.y;
            vmax.y = maxs.y;
        } else {
            vmin.y = maxs.y;
            vmax.y = mins.y;
        }
        // Z axis
        if(mFrustum[i].z > 0) {
            vmin.z = mins.z;
            vmax.z = maxs.z;
        } else {
            vmin.z = maxs.z;
            vmax.z = mins.z;
        }

        if(glm::dot(normal,vmin)   + w > 0){
            return true;
        }
        if(glm::dot(normal,vmax) + w < 0){
            return false;
        }
    }
    return false;
}


glm::vec2 Camera::wPos() const {
    return {mPosition.x,mPosition.y};
}
