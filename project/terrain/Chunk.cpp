#include "Chunk.h"

#include <glm/gtc/matrix_transform.hpp>
#include "World.h"


using namespace glm;

Chunk::Chunk(const glm::vec2& offset, const glm::vec2& size)
    : mRand(-1,1),mOffset(offset), mSize(size), mRes(-1), mModel(scale(translate(mat4(),vec3(mOffset,0)),vec3(mSize,1))) {
    mEng.seed(offset.x+offset.y);
}

void Chunk::update(float delta_s) {
}

int Chunk::setAttrs(int res,SharedTexture hmap, const Grid& terrain, const Grid &water, const Grid &grass) {
    mRes = res;
    mHmap = hmap;
    mTerrain = &terrain;
    mWater = &water;
    mGrass = &grass;

    return 1;
}

void Chunk::addTrees(Material& trunc, Material& leaves) {
    float sq = 8;
    mTrees.reserve(sq*sq);
    for(int i = 1; i < sq; i++) {
        for(int j = 1; j < sq; j++) {
            vec2 jitter = vec2(mRand(mEng),mRand(mEng))*18.f;
            vec2 rcpos = vec2(i*mSize.x / sq, j*mSize.y / sq)+jitter;
            vec2 texPos = rcpos*float(mHmap->res()) / mSize;
            float h = mHmap->valAt(texPos.x,texPos.y)-2;
            if(h<10 || h > 300) continue; //Don't generate trees out of range
            float hpx = mHmap->valAt(texPos.x+1,texPos.y);
            float hpy = mHmap->valAt(texPos.y,texPos.y+1);
            float hnx = mHmap->valAt(texPos.x-1,texPos.y);
            float hny = mHmap->valAt(texPos.y,texPos.y-1);
            vec3 normal = normalize(vec3(hnx-hpx,hny-hpy,4));
            mTrees.emplace_back(trunc,leaves);
            Tree& tree = mTrees.back();
            vec3 tpos = vec3(rcpos+pos(),h);
            float size = 35+mRand(mEng)*10;
            tree.build(tpos,normal*size,2.f);
        }
    }
}

glm::ivec2 Chunk::key() const {
    return mOffset/mSize;
}

void Chunk::setFrameID(long id) {
    mFrameId = id;
}

void Chunk::finish() {
    for(Tree& t : mTrees) {
        t.finish();
    }
}

void Chunk::drawTerrain(float time, const mat4 &view, const mat4 &projection, Material &mat, bool shad) {
    mTerrain->draw(time,mModel,view,projection,mat,1,mHmap,mHmap->res());
}

void Chunk::drawGrass(float time, const mat4 &view, const mat4 &projection,Material& mat) {
    mGrass->draw(time,mModel,view,projection,mat,1,mHmap,mHmap->res());
}

void Chunk::drawWater(float time, const mat4 &view, const mat4 &projection,Material& mat) {
    mWater->draw(time,mModel,view,projection,mat,1,mHmap,mHmap->res());
}

void Chunk::drawTruncs(float time, const glm::mat4& view, const glm::mat4& projection,Material& mat) {
    glUniformMatrix4fv(mat.uniformLocation("VP"), ONE, DONT_TRANSPOSE,glm::value_ptr(projection*view));
    glUniformMatrix4fv(mat.uniformLocation("V"), ONE, DONT_TRANSPOSE,glm::value_ptr(view));
    for(Tree& t : mTrees) {
        t.drawTrunc(view,projection,mat);
    }
}

void Chunk::drawLeaves(float time, const glm::mat4& view, const glm::mat4& projection,Material& mat) {
    glUniformMatrix4fv(mat.uniformLocation("VP"), ONE, DONT_TRANSPOSE,glm::value_ptr(projection*view));
    glUniformMatrix4fv(mat.uniformLocation("V"), ONE, DONT_TRANSPOSE,glm::value_ptr(view));
    glUniformMatrix4fv(mat.uniformLocation("iV"), ONE, DONT_TRANSPOSE,glm::value_ptr(inverse(view)));
    glUniform1f(mat.uniformLocation("time"),time);
    for(Tree& t : mTrees) {
        t.drawLeaves(view,projection,mat);
    }
}

Chunk::~Chunk() {
}
