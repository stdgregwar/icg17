#ifndef BEZIER_H
#define BEZIER_H

#include <vector>
using namespace std;

template<class K>
class Bezier
{
public:
    Bezier(vector<K> controlPoints): mControlPoints(controlPoints)
    {

    }

    K curveAtTime(float time) {
        vector<K> newPoints(mControlPoints);
        return bezier(newPoints,newPoints.size(),fmod(time,1));
    }

private:
    vector<K> mControlPoints;

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

#endif // BEZIER_H

