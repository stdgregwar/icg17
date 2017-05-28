#include "Chunk.h"

#include <glm/gtc/matrix_transform.hpp>
#include "World.h"


using namespace glm;

Chunk::Chunk(const glm::vec2& offset, const glm::vec2& size)
    : mRand(-1,1),mOffset(offset), mSize(size), mRes(-1), mModel(scale(translate(mat4(),vec3(mOffset,0)),vec3(mSize,1))) {
    mEng.seed(offset.x+189*offset.y);
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

void Chunk::addTrees(Material& trunc, Material& leaves, SimplexNoise& n, bool planeOnly) {
    float sq = 12;
    mEng.seed(mOffset.x+6649*mOffset.y);
    mTrees.reserve(sq*sq);
    for(int i = 1; i < sq; i++) {
        for(int j = 1; j < sq; j++) {
            vec2 jitter = vec2(mRand(mEng),mRand(mEng))*18.f;
            vec2 rcpos = vec2(i*mSize.x / sq, j*mSize.y / sq)+jitter;
            vec2 wpos = pos()+rcpos;

            float noi = n.fractal(2,wpos.x,wpos.y);

            if(noi < 0.1) continue; //Don't generate tree

            vec2 texPos = rcpos*float(mHmap->res()) / mSize;
            float h = mHmap->valAt(texPos.x,texPos.y);
            if(h<2 || h > 300) continue; //Don't generate trees out of range
            float hpx = mHmap->valAt(texPos.x+1,texPos.y);
            float hpy = mHmap->valAt(texPos.y,texPos.y+1);
            float hnx = mHmap->valAt(texPos.x-1,texPos.y);
            float hny = mHmap->valAt(texPos.y,texPos.y-1);
            float dz = mSize.x*1.f/mHmap->res();
            vec3 normal = normalize(vec3(hnx-hpx,hny-hpy,dz));
            float size = 35+mRand(mEng)*10;

            vec3 tpos = vec3(rcpos+pos(),h) - normal*2.f;

            if(!planeOnly) {
                mTrees.emplace_back(trunc,leaves);
                Tree& tree = mTrees.back();
                tree.build(tpos,normal*size,2.f+mRand(mEng)*0.5);
            } else {
                mRand(mEng); //Consume a rand to avoid desync
            }
            normal = normalize(normal*vec3(0.125,0.125,1));
            mTreePlanes.addTree(tpos,normal*size*1.6f);
        }
    }
    mTreePlanes.build();
}

glm::ivec2 Chunk::key() const {
    return mOffset/mSize;
}

void Chunk::setFrameID(long id) {
    mFrameId = id;
}

void Chunk::finish(Material &treePlanesMat) {
    for(Tree& t : mTrees) {
        t.finish();
    }
    mTreePlanes.finish(treePlanesMat);
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

void Chunk::drawTruncs(const vec3 &eye, float dist) {
    for(Tree& t : mTrees) {
        if(distance(eye,t.pos()) < dist) {
            t.drawTrunc();
        }
    }
}

void Chunk::drawLeaves(const vec3 &eye, float dist) {
    for(Tree& t : mTrees) {
        if(distance(eye,t.pos()) < dist) {
            t.drawLeaves();
        }
    }
}

void Chunk::drawPlanes() {
    mTreePlanes.draw();
}

Chunk::~Chunk() {
}
