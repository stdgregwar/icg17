#include "World.h"

using namespace glm;
using namespace std;

World::World(float chunkSize) : mChunkSize(chunkSize), mViewDistance(2), mFrameID(0), mPreviousCenter(5000,5000)
{

}

void World::init() {
    mNoise.init(mMaxRes);
    int res = mMaxRes;
    while(res > 2) {
        mTerrains.emplace(std::piecewise_construct,
                          std::forward_as_tuple(res),
                          std::forward_as_tuple());
        mTerrains[res].init(res);
        res = res >> 1;
    }
}

void World::update(float dt,const glm::vec2& worldPos) {
    i32vec2 center = worldPos/mChunkSize;

    int maxTasks = 16;
    for(int i = 0; i<maxTasks && mToDo.size();) {
        i+=mToDo.front()();
        mToDo.pop_front();
    }

    if(mPreviousCenter == center) return;

    mPreviousCenter = center;
    mFrameID++;
    int maxRes = mMaxRes;
    for(int x = center.x-mViewDistance; x <= center.x+mViewDistance; x++) {
        for(int y = center.y-mViewDistance; y <= center.y+mViewDistance; y++) {
            i32vec2 cpos = {x,y};
            int dist = std::min(std::max(0,std::max(abs(cpos.x-center.x),abs(cpos.y-center.y))-1),6);
            dist;
            int res = maxRes >> dist;
            Chunks::iterator it = mChunks.find(cpos);
            if(it == mChunks.end()) { //Chunk does not exist
                vec2 worldOffset = vec2(cpos)*mChunkSize;
                vec2 size = {mChunkSize,mChunkSize};

                pair<Chunks::iterator,bool> p = mChunks.emplace(std::piecewise_construct,
                                std::forward_as_tuple(cpos),
                                std::forward_as_tuple(worldOffset,size));
                it = p.first;
            }
            Chunk* c = &it->second;
            mToDo.push_back(
                        [this,c,res](){
                            return c->update(res,mFrameID,mNoise,mTerrains.at(res));
                        });
            //it->second.update(res,mFrameID,mNoise,mTerrains.at(res));
        }
    }

    /*for(Chunks::iterator it = mChunks.begin(); it != mChunks.end();) {
        if(it->second.frameID() != mFrameID) {
            it = mChunks.erase(it);
        } else {
            it++;
        }
    }*/
}

void World::draw(float time, const mat4 &view, const mat4 &projection) {
    for(auto& p : mChunks) {
        p.second.draw(time,view,projection);
    }
}
