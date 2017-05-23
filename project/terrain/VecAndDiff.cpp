#include "VecAndDiff.h"

VecAndDiff::VecAndDiff(const glm::vec3 v, const glm::vec3 d) : v(v), d(d)
{

}

VecAndDiff VecAndDiff::operator+(const VecAndDiff& other) const {
    return VecAndDiff(other.v + v, other.d + d);
}

VecAndDiff VecAndDiff::operator-(const VecAndDiff& other) const {
    return VecAndDiff(v-other.v, d-other.d);
}

VecAndDiff VecAndDiff::operator*(float s) const {
    return VecAndDiff(v*s,d*s);
}

