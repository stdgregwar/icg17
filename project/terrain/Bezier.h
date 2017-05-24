#ifndef BEZIER_H
#define BEZIER_H

#include <vector>
#include <cmath>
#include <glm/vec3.hpp>
#include "VecAndDiff.h"

using namespace std;

template<class K>
class Bezier
{
public:
    Bezier(vector< vector<K> > controlPoints): mControlPoints(controlPoints)
    {

    }

    K curveAtTime(float time) {
        int idx = ((int) floor(time))%mControlPoints.size();
        vector<K> newPoints(mControlPoints[idx]);
        return bezier(newPoints,newPoints.size(),fmod(time,1.f));
    }

    const K& firstPoint() const {
        return mControlPoints[0][0];
    }

private:
    vector< vector<K> > mControlPoints;

    K lerp(const K& a, const K& b, const float t) {
        return a + (b-a)*t;
    }

    K bezier(vector<K>& points,int maxIdx, float time) {
        if(maxIdx == 1){
            return points[0];
        } else {
            for(int i = 0; i < maxIdx; ++i){
                points[i] = lerp(points[i],points[i+1],time);
            }
            return bezier(points,maxIdx-1, time);
        }
    }
};

class BezierVecAndDiffBuilder
{
public:
    void addPoint(const VecAndDiff& point){
        mControlPoints.push_back(point);
//        vector<VecAndDiff> points;
//        // If mControlPoints is empty, we initialize it
//        if(mControlPoints.size() == 0){
//            points.push_back(point);
//        } else {
//            VecAndDiff lastPoint = mControlPoints[0][0];
//            // If we only have one point
//            if(mControlPoints.size() == 1 && mControlPoints[0].size() == 1){
//                points = mControlPoints[0];
//            } else {
//                 lastPoint = mControlPoints.back().back();
//                 points.push_back(lastPoint);
//            }
//            // We add a control point at average height and at mix between last and current point
//            glm::vec3 v(lastPoint.v.x,point.v.y,(lastPoint.v.z+point.v.z)/2.f);
//            glm::vec3 d(lastPoint.d.x,point.d.y,(lastPoint.d.z+point.d.z)/2.f);
//            VecAndDiff p(v,d);
//            points.push_back(p);
//            // We add the current point
//            points.push_back(point);
//        }
//        if(mControlPoints.size() == 1 && mControlPoints[0].size() == 1){
//            mControlPoints[0] = points;
//        }   else {
//            mControlPoints.push_back(points);
//        }
    }


    void reset() {
        mControlPoints.clear();
    }

    Bezier<VecAndDiff> build(){
            // We close the loop
        vector< vector< VecAndDiff > > controlPoints;
        size_t size = mControlPoints.size();
        float curve = 0.25;
        for(int i = 0; i < size+2; i++){
            vector<VecAndDiff> bez; bez.reserve(4);
            VecAndDiff curr = mControlPoints[i%size];
            VecAndDiff prev = mControlPoints[((i+size-1)%size)];
            VecAndDiff next = mControlPoints[(i+1)%size];
            VecAndDiff nextNext = mControlPoints[(i+2)%size];
            bez.push_back(curr);
            bez.push_back(curr+(next-prev)*curve);
            bez.push_back(next-(nextNext-curr)*curve);
            bez.push_back(next);
            controlPoints.push_back(bez);
        }

        return Bezier<VecAndDiff>(controlPoints);
    }

private:
    vector<VecAndDiff> mControlPoints;
};

#endif // BEZIER_H

