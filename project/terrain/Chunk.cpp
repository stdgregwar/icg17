#include "Chunk.h"

#include <glm/gtc/matrix_transform.hpp>
#include "World.h"


using namespace glm;

Chunk::Chunk(const glm::vec2& offset, const glm::vec2& size)
    : mOffset(offset), mSize(size), mRes(-1), mReady(false), mTexJob(nullptr), mHmap(0), mNextHmap(0) ,mTTime(0){

}

void Chunk::update(float delta_s) {
    constexpr float t_max = 0.25f;
    if(mTexFuture.valid()) {
        if(mTexFuture.wait_for(std::chrono::microseconds(1)) == future_status::ready) {
            //Texture is available!
            mNextHmap = mTexFuture.get();
            mTexFuture = TexFuture();
            mTexJob = nullptr;
        }
    }
    if(mNextHmap) { //Do transition
        mTTime += delta_s;
        mNextAlpha =  std::min(1.f,mTTime*2/t_max);
        mAlpha = std::min(1.f,2-mTTime*2/t_max);
        if(mTTime > t_max) {
            mAlpha = 1;
            mHmap = mNextHmap;
            mNextHmap = 0;
            mRes = mNextRes;
            mTerrain = mNextTerrain;
            mWater = mNextWater;
            mGrass = mNextGrass;
            mReady = true;
            mTTime = 0;
        }
    }
}

int Chunk::updateRes(int res, TexGenerator &texGen, const Grid& terrain, const Grid &water, const Grid &grass) {


    if(mRes == res) return 0;
    mNextRes = res;
    mNextTerrain = &terrain;
    mNextWater = &water;
    mNextGrass = &grass;

    //if(mTexFuture.valid()) mTexFuture.get(); //Throw future result
    //if(mTexJob) mTexJob->valid = false; //invalidate previous job
    TexGenerator::Job* job;
    mTexFuture = texGen.getTexture({res*8+2,res*8+2},
                                   [res,this](ScreenQuad& q){
                                        mat4 model = translate(mat4(),vec3(mOffset,0));
                                        model = scale(model,vec3(mSize,mSize.x));
                                        q.draw(model,res);},
                                   job
                                   );
    //if(mTexJob) mTexJob->valid = false;
    mTexJob = job;
    return 1;
}

void Chunk::setFrameID(long id) {
    mFrameId = id;
}

void Chunk::drawTerrain(float time, const mat4 &view, const mat4 &projection) {
    //if(!mReady) return;
    mat4 model = translate(mat4(),vec3(mOffset,0));
    model = scale(model,vec3(mSize,mSize.x*(32/mSize.x)));

    if(mHmap) {
        mTerrain->draw(time,model,view,projection,mAlpha,mHmap,mRes);
    }
    //Transition
    if(mNextHmap) {
        mNextTerrain->draw(time,model,view,projection,mNextAlpha,mNextHmap,mNextRes);
    }
}

void Chunk::drawGrass(float time, const mat4 &view, const mat4 &projection) {
    if(!mReady) return;
    mat4 model = translate(mat4(),vec3(mOffset,0));
    model = scale(model,vec3(mSize,mSize.x*(32/mSize.x)));
    mGrass->draw(time,model,view,projection,1,mHmap,mRes);
}

void Chunk::drawWater(float time, const mat4 &view, const mat4 &projection) {
    if(!mReady) return;
    mat4 model = translate(mat4(),vec3(mOffset,0));
    model = scale(model,vec3(mSize,mSize.x*(32/mSize.x)));
    mWater->draw(time,model,view,projection,1,mHmap,mRes);
}

Chunk::~Chunk() {
}
