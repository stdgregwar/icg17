#include "World.h"

using namespace glm;
using namespace std;

World::World(float chunkSize) : mChunkSize(chunkSize), mViewDistance(16),
    mFrameID(0), mCenter(5000,5000), mMaxRes(128), mTaskPerFrame(8)
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
    int remaining = mToDo.size();
    for(int i = 0; i<mTaskPerFrame && mToDo.size();) {
        ChunkTask& t = mToDo.front();
        switch (t.type) {
        case ChunkTask::CREATE:
            //TODO
            break;
        case ChunkTask::UPDATE:{
            auto it = mChunks.find(t.chunk);
            if(it == mChunks.end()) {
                throw std::runtime_error("Update on null chunk!");
            }
            i+=t.task(&it->second);
            break;
        }
        case ChunkTask::DELETE:
            mChunks.erase(t.chunk);
            //i++;
            break;
        default:
            break;
        }
        mToDo.pop_front();
    }

    if(mCenter != center) {
        mCenter = center;
        updateChunks();
    }

    if(mToDo.size()-remaining > mViewDistance*mTaskPerFrame) {
        mTaskPerFrame = std::min(mViewDistance*mViewDistance,mTaskPerFrame+mViewDistance/8);
    } else {
        mTaskPerFrame = std::max(4, mTaskPerFrame-1);
    }
    cout << "taskspf " << mTaskPerFrame << endl;
}

void World::updateChunks() {
    i32vec2 center = mCenter;
    mFrameID++;
    int maxRes = mMaxRes;
    for(int x = center.x-mViewDistance; x <= center.x+mViewDistance; x++) {
        for(int y = center.y-mViewDistance; y <= center.y+mViewDistance; y++) {
            i32vec2 cpos = {x,y};
            int dist = std::min(std::max(0,std::max(abs(cpos.x-center.x),abs(cpos.y-center.y))-2),5);
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
            c->setFrameID(mFrameID);
            pushTask({
                         ChunkTask::UPDATE,
                         cpos,
                        [this,res](Chunk* c){
                            return c->update(res,mNoise,mTerrains.at(res));
                        }});
            //it->second.update(res,mFrameID,mNoise,mTerrains.at(res));
        }
    }
    for(Chunks::value_type& p : mChunks) {
        if(p.second.frameID() != mFrameID) {
            pushTask({ChunkTask::DELETE,p.second.pos()/mChunkSize,nullptr});
        }
    }
}

void World::pushTask(ChunkTask task) {
    mToDo.remove_if([&task](ChunkTask& ct){return ct.chunk == task.chunk && ct.type != ChunkTask::CREATE;});
    mToDo.push_back(task);
}

void World::draw(float time, const mat4 &view, const mat4 &projection) {
    for(auto& p : mChunks) {
        p.second.draw(time,view,projection);
    }
}
