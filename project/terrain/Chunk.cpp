#include "Chunk.h"

#include <glm/gtc/matrix_transform.hpp>
#include "World.h"


using namespace glm;

Chunk::Chunk(const glm::vec2& offset, const glm::vec2& size)
    : mOffset(offset), mSize(size), mRes(-1), mReady(false), mTexJob(nullptr) {

}

void Chunk::update(float delta_s) {
    if(mTexFuture.valid()) {
        if(mTexFuture.wait_for(std::chrono::microseconds(1)) == future_status::ready) {
            //Texture is available!
            mHmap = mTexFuture.get();
            mRes = mNextRes;
            mTerrain = mNextTerrain;
            mWater = mNextWater;
            mReady = true;
            mTexJob = nullptr;
        }
    }
}

int Chunk::updateRes(int res, TexGenerator &texGen, const Grid& terrain, const Grid &water) {


    if(mRes == res) return 0;
    mNextRes = res;
    mNextTerrain = &terrain;
    mNextWater = &water;

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
    if(!mReady) return;
    mat4 model = translate(mat4(),vec3(mOffset,0));
    model = scale(model,vec3(mSize,mSize.x*(32/mSize.x)));
    mTerrain->draw(time,model,view,projection,mHmap,mRes);
}

void Chunk::drawWater(float time, const mat4 &view, const mat4 &projection) {
    if(!mReady) return;
    mat4 model = translate(mat4(),vec3(mOffset,0));
    model = scale(model,vec3(mSize,mSize.x*(32/mSize.x)));
    mWater->draw(time,model,view,projection,mHmap,mRes);
}

Chunk::~Chunk() {
}
