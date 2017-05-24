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
        vector<VecAndDiff> points;
        // If mControlPoints is empty, we initialize it
        if(mControlPoints.size() == 0){
            points.push_back(point);
        } else {
            VecAndDiff lastPoint = mControlPoints[0][0];
            // If we only have one point
            if(mControlPoints.size() == 1 && mControlPoints[0].size() == 1){
                points = mControlPoints[0];
            } else {
                 lastPoint = mControlPoints.back().back();
                 points.push_back(lastPoint);
            }
            // We add a control point at average height and at mix between last and current point
            glm::vec3 v(lastPoint.v.x,point.v.y,(lastPoint.v.z+point.v.z)/2.f);
            glm::vec3 d(lastPoint.d.x,point.d.y,(lastPoint.d.z+point.d.z)/2.f);
            VecAndDiff p(v,d);
            points.push_back(p);
            // We add the current point
            points.push_back(point);
        }
        if(mControlPoints.size() == 1 && mControlPoints[0].size() == 1){
            mControlPoints[0] = points;
        }   else {
            mControlPoints.push_back(points);
        }
    }

    void reset() {
        vector< vector<VecAndDiff> > points;
        mControlPoints = points;
    }

    Bezier<VecAndDiff> build(){
        if(mControlPoints.size() != 0){
            // We close the loop
            addPoint(mControlPoints[0][0]);
        }
        return Bezier<VecAndDiff>(mControlPoints);
    }

private:
    vector< vector<VecAndDiff> > mControlPoints;
};

#endif // BEZIER_H

