#include "Material.h"

Material::Material() : mProgramId(0) {

}

Material::Material(const std::string& vshader, const std::string& fshader) {
    init(vshader,fshader);
}

void Material::init(const string& vshader, const string& fshader) {
    mProgramId = icg_helper::LoadShaders(vshader.c_str(),
                                              fshader.c_str());

    mTextures.clear();

    if(!mProgramId) {
        throw std::runtime_error("could not get shaders \"" + vshader + "\" and/or \"" + fshader + "\" to compile");
    }
}

GLuint Material::attrLocation(const string& name) {
    return glGetAttribLocation(mProgramId,name.c_str());
}

GLuint Material::uniformLocation(const string& name) {
    return glGetUniformLocation(mProgramId,name.c_str());
}

GLuint Material::addTexture(GLuint no, const string& filename, const string& uName, GLuint filter,GLuint repeat,bool genMipmaps) {
    int width;
    int height;
    int nb_component;
    // set stb_image to have the same coordinates as OpenGL
    stbi_set_flip_vertically_on_load(1);
    unsigned char* image = stbi_load(filename.c_str(), &width,
                                     &height, &nb_component, 0);

    if(image == nullptr) {
        throw std::runtime_error("Failed to load texture");
    }
    addTexture(GL_TEXTURE_2D,no,image,nb_component == 3 ? GL_RGB : GL_RGBA,GL_UNSIGNED_BYTE,{width,height},uName,filter,repeat,genMipmaps);
    stbi_image_free(image);
}

GLuint Material::addTexture(GLuint target, GLuint no, const void* data, GLuint format, GLuint type, const vector<size_t>& dim, const string& uName, GLuint filter, GLuint repeat, bool genMipmaps) {
    glUseProgram(mProgramId);
    GLuint tid;
    glGenTextures(1,&tid);
    glActiveTexture(no);
    glBindTexture(target, tid);
    switch(target) {
    case GL_TEXTURE_1D:
        glTexImage1D(GL_TEXTURE_1D, 0, format, dim[0], 0, format, type, data);
        break;
    case GL_TEXTURE_2D:
        glTexImage2D(GL_TEXTURE_2D, 0, format, dim[0],dim[1], 0, format, type, data);
        break;
    }
    glTexParameteri(target, GL_TEXTURE_MAG_FILTER, filter);
    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, filter);
    glTexParameterf( target, GL_TEXTURE_WRAP_S, repeat);
    glTexParameterf( target, GL_TEXTURE_WRAP_T, repeat);

    if(genMipmaps) {
        glGenerateMipmap(target);
    }

    glBindTexture(target,0);
    glUseProgram(0);
    mTextures.push_back({
                            uName,
                            tid,
                            target,
                            no,
                            uniformLocation(uName)
                        });
    return tid;
}

GLuint Material::addTexture(GLuint target, GLuint no,GLuint texId,const string& uName) {
    glUseProgram(mProgramId);
    glActiveTexture(no);
    glBindTexture(target, texId);
    mTextures.push_back({
                            uName,
                            texId,
                            target,
                            no,
                            uniformLocation(uName)
                        });
    return texId;
}


void Material::bind() {
    glUseProgram(mProgramId);
    for(Texture& t : mTextures) {
        glActiveTexture(t.no);
        glBindTexture(t.target, t.id);
        glUniform1i(t.loc,t.no-GL_TEXTURE0);
    }
}

void Material::unbind() {
    glUseProgram(0);
}

Material::~Material() {

}
