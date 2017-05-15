#ifndef SHADERBUILDER_H
#define SHADERBUILDER_H

#include <icg_helper.h>

namespace ShaderBuilder {
    string makeSource(const string& file);
    GLuint makeShader(const string& vshader, const string& fshader, const string& gshader = "");
}
#endif // SHADERBUILDER_H
