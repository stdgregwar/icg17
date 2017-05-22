#include "Chunk.h"

#include <glm/gtc/matrix_transform.hpp>
#include "World.h"


using namespace glm;

Chunk::Chunk(const glm::vec2& offset, const glm::vec2& size)
    : mOffset(offset), mSize(size), mRes(-1), mReady(false), mTexJob(nullptr) ,mTTime(0), mModel(scale(translate(mat4(),vec3(mOffset,0)),vec3(mSize,1))) {
}

void Chunk::update(float delta_s) {
    constexpr float t_max = 0.5f;
    if(mTexFuture.valid()) {
        if(mTexFuture.wait_for(std::chrono::microseconds(1)) == future_status::ready) {
            //Texture is available!
            mNextHmap = mTexFuture.get();
            mTexFuture = TexFuture();
            mTexJob = nullptr;
        }
    }
    if(mNextHmap.get()) { //Do transition
        /*mTTime += delta_s;
        mNextAlpha =  std::min(1.f,mTTime*2/t_max);
        mAlpha = std::min(1.f,2-mTTime*2/t_max);
        if(mTTime > t_max) {*/
            mAlpha = 1;
            mHmap = mNextHmap;
            mNextHmap.reset();
            mRes = mNextRes;
            mTerrain = mNextTerrain;
            mWater = mNextWater;
            mGrass = mNextGrass;
            mReady = true;
            mTTime = 0;
        //}
    }
}

int Chunk::updateRes(int res, ChunkTexGenerator &texGen, const Grid& terrain, const Grid &water, const Grid &grass) {
    size_t tres = res*8+2;
    if(mRes == tres) return 0;
    mNextRes = tres;
    mNextTerrain = &terrain;
    mNextWater = &water;
    mNextGrass = &grass;


    //if(mTexFuture.valid()) mTexFuture.get(); //Throw future result
    //if(mTexJob) mTexJob->valid = false; //invalidate previous job
    ChunkTexGenerator::Job* job;

    /*mTexFuture = texGen.getTexture({tres,tres},
                                   [tres,this](ScreenQuad& q){
                                        q.draw(mModel,tres);},
                                   job
                                   );*/
    glm::ivec3 key(mOffset/mSize,tres);
    mTexFuture = texGen.getChunkTex(key,mSize.x,job);
    //if(mTexJob) mTexJob->valid = false;
    mTexJob = job;
    return 1;
}

void Chunk::setFrameID(long id) {
    mFrameId = id;
}

void Chunk::drawTerrain(float time, const mat4 &view, const mat4 &projection, Material &mat, bool shad) {
    //if(!mReady) return;
    if(mHmap.get()) {
        mTerrain->draw(time,mModel,view,projection,mat,mAlpha,mHmap,mRes);
    }
    //Transition
    /*if(mNextHmap.get()) {
        mNextTerrain->draw(time,mModel,view,projection,mat,mNextAlpha,mNextHmap,mNextRes);
    }*/
}

void Chunk::drawGrass(float time, const mat4 &view, const mat4 &projection,Material& mat) {
    if(!mReady) return;
    mGrass->draw(time,mModel,view,projection,mat,1,mHmap,mRes);
}

void Chunk::drawWater(float time, const mat4 &view, const mat4 &projection,Material& mat) {
    if(!mReady) return;
    mWater->draw(time,mModel,view,projection,mat,1,mHmap,mRes);
}

Chunk::~Chunk() {
}
