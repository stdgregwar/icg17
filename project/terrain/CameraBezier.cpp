#include "CameraBezier.h"
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <unordered_map>
#include <iostream>

#ifndef M_PI
#define M_PI 3.1415
#endif

using namespace glm;
using namespace std;



CameraBezier::CameraBezier(const glm::vec3& pos, const glm::vec3& orientation, const Bezier<VecAndDiff>& bezier): Camera(pos,orientation), mSSpeed(20), mLSpeed(1),
    mTargetRotation(orientation), mTargetPosition(pos), mBezierPath(bezier), mBezierSpeed({{1.0f,2.0f,30.0f,1.0f}})
{

}

CameraBezier::CameraBezier(const Bezier<VecAndDiff>& bezier): CameraBezier(bezier.firstPoint().v,bezier.firstPoint().d,bezier)
{

}

CameraBezier::CameraBezier(const vec3 &pos, const vec3 &orientation, const vector< vector<VecAndDiff> > &pathControlPoints) : CameraBezier(pos, orientation, Bezier<VecAndDiff>(pathControlPoints))
{

}



void CameraBezier::setBaseSpeed(float speed) {
    mSSpeed = speed;
}

void CameraBezier::update(float delta_s, const Chunk& c) {
    static float time = 0;
    float speed = mBezierSpeed.curveAtTime(time*0.01)+mLSpeed.x;
    time += delta_s*speed;
    float dtime = time*0.01;
//    cout << "dtime: " << dtime << " speed: " << speed << endl;
    VecAndDiff point = mBezierPath.curveAtTime(dtime);
//    cout << "time: " << dtime << " x: " << point.v.x << " y: " << point.v.y << endl;
    mTargetRotation = point.d;
    mRotation = mRotation + (mTargetRotation - mRotation) * std::min(10.f * delta_s,1.f);

    vec3& rot = mRotation;
    vec3 look = {cos(rot.y)*cos(rot.x),
                 cos(rot.y)*sin(rot.x),
                 sin(rot.y)};

    vec3 up = vec3(0,0,1);
    vec3 side = normalize(cross(look,up));
    vec3 wspeed = look*mLSpeed.x + side*mLSpeed.y;

    mTargetPosition += wspeed*mSSpeed*delta_s;
    mTargetPosition = point.v;
    mPosition = mPosition + (mTargetPosition - mPosition) * std::min(10.f * delta_s,1.f);
    mView = lookAt(mPosition,mPosition+look,up);
}

void CameraBezier::rotate(glm::vec2 delta) {
    mTargetRotation.x += delta.x;
    static float max = M_PI/2-0.1;
    mTargetRotation.y += delta.y;
    if(abs(mTargetRotation.y) > max) {
        mTargetRotation.y = sign(mTargetRotation.y)*max;
    }
}

void CameraBezier::speed(glm::vec2 delta) {
    mLSpeed.x += delta.x;
    mLSpeed.y += delta.y;
}


void CameraBezier::onKey(GLFWwindow* window, int key, int scancode, int action, int mods) {
    unordered_map<int,vec2> pressed{
        {GLFW_KEY_W,{1,0}},
        {GLFW_KEY_S,{-1,0}},
        {GLFW_KEY_A,{0,-1}},
        {GLFW_KEY_D,{0,1}}
    };
    if(pressed.count(key)) {
        if(action == GLFW_PRESS) {
            mLSpeed.x += pressed.at(key).x*0.5 ;
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

void CameraBezier::onMouse(GLFWwindow* window, double xpos, double ypos) {
    static float lx = 0,ly =0;
    float xrel = xpos-lx;
    lx = xpos;
    float yrel = ypos-ly;
    ly = ypos;

    rotate(vec2{-xrel,-yrel}*0.001f);
}
