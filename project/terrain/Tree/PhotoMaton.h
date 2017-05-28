#ifndef PHOTOMATON_H
#define PHOTOMATON_H

#include "../GBuffer.h"
#include "Tree.h"

class PhotoMaton
{
public:
    PhotoMaton();
    void generate(size_t treeCount, size_t baseRes, Material& trunc, Material& leaves);
    void draw(size_t treeCount, size_t baseRes,Material& trunc, Material& leaves);
    GLuint normal() {return mGBuffer.normal();}
    GLuint color() {return mGBuffer.diffuse();}
private:
    vector<Tree> mTrees;
    GBuffer mGBuffer;
};

#endif // PHOTOMATON_H
