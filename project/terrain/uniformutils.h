#pragma once

#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Material.h"

namespace Uniforms {

using namespace glm;

inline void viewProjTime(Material& m,const mat4& view, const mat4& proj, float time) {
    m.bind();
    glUniform1f(m.uniformLocation("time"),time);
    glUniformMatrix4fv(m.uniformLocation("P"), ONE, DONT_TRANSPOSE,glm::value_ptr(proj));
    glUniformMatrix4fv(m.uniformLocation("VP"), ONE, DONT_TRANSPOSE,glm::value_ptr(proj*view));
    glUniformMatrix4fv(m.uniformLocation("V"), ONE, DONT_TRANSPOSE,glm::value_ptr(view));
    glUniformMatrix4fv(m.uniformLocation("iV"), ONE, DONT_TRANSPOSE,glm::value_ptr(inverse(view)));
}

}
