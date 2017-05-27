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
    Bezier(vector< vector<K> > controlPoints = {}): mControlPoints(controlPoints)
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

template<class K>
class BezierBuilder
{
public:
    void addPoint(const K& point){
        mControlPoints.push_back(point);
    }


    template<class... Types>
    BezierBuilder& operator()(const Types& ... args) {
        addPoint(K(args...));
        return *this;
    }

    void reset() {
        mControlPoints.clear();
    }

    Bezier<K> build(){
            // We close the loop
        vector< vector< K > > controlPoints;
        size_t size = mControlPoints.size();
        float curve = 0.25;
        for(int i = 0; i < size; i++){
            vector<K> bez; bez.reserve(4);
            K curr = mControlPoints[i%size];
            K prev = mControlPoints[((i+size-1)%size)];
            K next = mControlPoints[(i+1)%size];
            K nextNext = mControlPoints[(i+2)%size];
            bez.push_back(curr);
            bez.push_back(curr+(next-prev)*curve);
            bez.push_back(next-(nextNext-curr)*curve);
            bez.push_back(next);
            controlPoints.push_back(bez);
        }

        return Bezier<K>(controlPoints);
    }

private:
    vector<K> mControlPoints;
};

#endif // BEZIER_H

