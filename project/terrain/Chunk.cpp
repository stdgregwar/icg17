#include "Chunk.h"

#include <glm/gtc/matrix_transform.hpp>
#include "World.h"


using namespace glm;

Chunk::Chunk(const glm::vec2& offset, const glm::vec2& size) : mOffset(offset), mSize(size), mRes(-1), mReady(false) {

}

int Chunk::update(int res, const ScreenQuad &noise, const Grid& terrain, const Grid &water) {
    if(mRes == res) return 0;
    mRes = res;


    mHmap = mNoiseBuffer.init(res*4+2,res*4+2);
    mNoiseBuffer.bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    mat4 model = translate(mat4(),vec3(mOffset,0));
    model = scale(model,vec3(mSize,mSize.x));
    noise.draw(model,res);
    mNoiseBuffer.unbind();
    mTerrain = &terrain;
    mWater = &water;
    mReady = true;
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
