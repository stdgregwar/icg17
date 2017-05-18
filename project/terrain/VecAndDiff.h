#ifndef VECANDDIFF_H
#define VECANDDIFF_H

#include <glm/vec3.hpp>

class VecAndDiff
{
public:
    VecAndDiff(const glm::vec3 v, const glm::vec3 d);
    VecAndDiff operator+(const VecAndDiff& other) const;
    VecAndDiff operator*(float s) const;
private:
    glm::vec3 v;
    glm::vec3 d;
};

#endif // VECANDDIFF_H
