#include "PhotoMaton.h"
#include <glm/gtc/matrix_transform.hpp>
#include "../uniformutils.h"

using namespace glm;

PhotoMaton::PhotoMaton()
{

}

void PhotoMaton::generate(size_t treeCount, size_t baseRes, Material& trunc, Material& leaves) {
    float posShift = 256;
    float treeSize = 40;
    for(int i = 0; i < treeCount; i++) {
        vec3 pos(posShift*(i+4),0,0);
        mTrees.emplace_back(trunc,leaves);
        Tree& tree = mTrees.back();
        tree.build(pos,vec3(0,0,treeSize),3.f);
        tree.finish(); //Finish them directly
    }

    mGBuffer.init(treeCount*baseRes,baseRes);
    draw(treeCount,baseRes,trunc,leaves);
}

void PhotoMaton::draw(size_t treeCount, size_t baseRes,Material& trunc, Material& leaves) {
    float posShift = 256;
    float treeSize = 40;
    float camSide = treeSize*0.75 + 2;

    mat4 proj = ortho(-camSide,camSide,-camSide,camSide,-camSide,camSide);


    mGBuffer.bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for(int i = 0; i < treeCount; i++) {
        Tree& t = mTrees.at(i);
        glViewport(i*baseRes,0,baseRes,baseRes);
        vec3 pos = t.pos() + vec3(0,0,treeSize/2+10);
        vec3 eye = t.pos() + vec3(0,-2,treeSize/2+10);
        mat4 view = lookAt(eye,pos,vec3(0,0,1));
         //Draw only in right spot
        Uniforms::viewProjTime(trunc,view,proj,0);
        mTrees.at(i).drawTrunc();
        Uniforms::viewProjTime(leaves,view,proj,0);
        mTrees.at(i).drawLeaves();
    }
    mGBuffer.unbind();

    glBindTexture(GL_TEXTURE_2D,mGBuffer.diffuse());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D,mGBuffer.normal());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D,0);
}
