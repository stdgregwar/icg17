#include "Chunk.h"

#include <glm/gtc/matrix_transform.hpp>
#include "World.h"

using namespace glm;

Chunk::Chunk(const glm::vec2& offset, const glm::vec2& size) : mOffset(offset), mSize(size), mRes(-1), mReady(false) {

}

void Chunk::update(int res, long frameid, const NoiseGen& noise, const Terrain& terrain) {
    mFrameId = frameid;
    if(mRes == res) return;
    mRes = res;


    mHmap = mNoiseBuffer.init(res,res);
    mNoiseBuffer.bind();
    mat4 model = translate(mat4(),vec3(mOffset,0));
    model = scale(model,vec3(mSize,mSize.x));
    noise.draw(model);
    mNoiseBuffer.unbind();
    mTerrain = &terrain;
    mReady = true;
}

void Chunk::draw(float time, const mat4 &view, const mat4 &projection) {
    if(!mReady) return;
    mat4 model = translate(mat4(),vec3(mOffset,0));
    model = scale(model,vec3(mSize,mSize.x));
    mTerrain->draw(time,model,view,projection,mHmap);
}

Chunk::~Chunk() {
}
