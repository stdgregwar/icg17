#include "Chunk.h"

#include <glm/gtc/matrix_transform.hpp>
#include "World.h"


using namespace glm;

Chunk::Chunk(const glm::vec2& offset, const glm::vec2& size)
    : mOffset(offset), mSize(size), mRes(-1), mReady(false), mTexJob(nullptr), mModel(scale(translate(mat4(),vec3(mOffset,0)),vec3(mSize,mSize.x*(32/mSize.x)))) {
}

void Chunk::update(float delta_s) {
    if(mTexFuture.valid()) {
        if(mTexFuture.wait_for(std::chrono::microseconds(1)) == future_status::ready) {
            //Texture is available!
            mHmap = mTexFuture.get();
            mRes = mNextRes;
            mTerrain = mNextTerrain;
            mWater = mNextWater;
            mGrass = mNextGrass;
            mReady = true;
            mTexJob = nullptr;
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
                                        q.draw(mModel,res);},
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
    mTerrain->draw(time,mModel,view,projection,mHmap,mRes);
}

void Chunk::drawGrass(float time, const mat4 &view, const mat4 &projection) {
    if(!mReady) return;
    mGrass->draw(time,mModel,view,projection,mHmap,mRes);
}

void Chunk::drawWater(float time, const mat4 &view, const mat4 &projection) {
    if(!mReady) return;
    mWater->draw(time,mModel,view,projection,mHmap,mRes);
}

bool Chunk::isInFrustum(const glm::mat4 &VP) {
    for(int x = 0; x <= 1; x++) {
        for(int y = 0; y<= 1; y++) {
            for(int z = -1; z <= 1; z += 2) {
                glm::vec4 pos4D = VP*mModel*glm::vec4(pos()+glm::vec2(x,y),2*z,1.0);
                glm::vec3 pos3D = glm::vec3(pos4D.x,pos4D.y,pos4D.z)/pos4D.w;
                if(abs(pos3D.x) <= 1.0 || abs(pos3D.y) <= 1.0 || abs(pos3D.z) <= 1.0) {
                    return true;
                }
            }
        }
    }

    return false;
}

Chunk::~Chunk() {
}
