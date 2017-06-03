#include "Chunk.h"
#include "CameraFreefly.h"
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <unordered_map>


#ifndef M_PI
#define M_PI 3.1415
#endif

using namespace glm;
using namespace std;

CameraFreefly::CameraFreefly(const vec3 &pos, const vec3 &orientation) : Camera(pos,orientation), mSSpeed(250), mLSpeed(0), mTargetRotation(orientation), mTargetPosition(pos), mGravity(false)
{

}

void CameraFreefly::update(float delta_s, const Chunk& c) {
    mRotation = mRotation + (mTargetRotation - mRotation) * std::min(5.f * delta_s,1.f);

    vec3& rot = mRotation;
    vec3 look = {cos(rot.y)*cos(rot.x),
                 cos(rot.y)*sin(rot.x),
                 sin(rot.y)};

    vec3 up = vec3(0,0,1);
    vec3 side = normalize(cross(look,up));
    float fac = mGravity ? 0.1 : 1;
    vec3 wspeed = look*mLSpeed.x*fac + side*mLSpeed.y*fac;
    if(mGravity) {
        wspeed.z -= 9.81*delta_s;
    }

    mTargetPosition += wspeed*mSSpeed*delta_s;
    mTargetPosition = inMap(mTargetPosition,c);
    mPosition += (mTargetPosition - mPosition) * std::min(5.f * delta_s,1.f);
    mPosition = inMap(mPosition,c);

    mView = lookAt(mPosition,mPosition+look,up);
}

void CameraFreefly::rotate(glm::vec2 delta) {
    mTargetRotation.x += delta.x;
    static float max = M_PI/2-0.1;
    mTargetRotation.y += delta.y;
    if(abs(mTargetRotation.y) > max) {
        mTargetRotation.y = sign(mTargetRotation.y)*max;
    }
}

void CameraFreefly::speed(glm::vec2 delta) {
    mLSpeed.x += delta.x;
    mLSpeed.y += delta.y;
}


void CameraFreefly::onKey(GLFWwindow* window, int key, int scancode, int action, int mods) {
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

void CameraFreefly::onMouse(GLFWwindow* window, double xpos, double ypos) {
    static float lx = 0,ly =0;
    float xrel = xpos-lx;
    lx = xpos;
    float yrel = ypos-ly;
    ly = ypos;

    rotate(vec2{-xrel,-yrel}*0.001f);
}
