#ifndef TEXTURE_H
#define TEXTURE_H

#include <icg_helper.h>
#include <memory>
#include <iostream>

/**
 * @brief The Texture class destroy automatically the texture
 */
class GCTexture {
public:
    GCTexture(GLuint tex) : mID(tex) {}
    GCTexture(const GCTexture& other) = delete;
    GCTexture& operator=(const GCTexture& other) = delete;
    GLuint id() const {return mID;}
    ~GCTexture() {
        glDeleteTextures(1,&mID);
    }
private:
    GLuint mID;
};


class SharedTexture : public shared_ptr<GCTexture> {
public:
    SharedTexture(GLuint tex) : shared_ptr(new GCTexture(tex)) {}
    SharedTexture() : shared_ptr() {}
    operator GLuint() {return this->get()->id();}
};

#endif // TEXTURE_H
