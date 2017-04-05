#ifndef WORLD_H
#define WORLD_H

#include <unordered_map>
#include <glm/gtx/hash.hpp>
#include "Chunk.h"
#include "NoiseGen/NoiseGen.h"
#include <list>
#include <functional>

typedef std::unordered_map<glm::i32vec2,Chunk> Chunks;
typedef std::unordered_map<int,Terrain> Terrains;

struct ChunkTask {
    enum Type{
        CREATE,
        UPDATE,
        DELETE
    };
    Type type;
    glm::i32vec2 chunk;
    std::function<int(Chunk* c)> task;
};

typedef std::list<ChunkTask> Tasks;

class World
{
public:
    World(float chunkSize);
    void init();
    void setViewDistance(int chunks);
    void update(float dt,const glm::vec2& worldPos);
    void draw(float time,const glm::mat4& view, const glm::mat4& projection);
    void pushTask(ChunkTask task);
private:
    Terrains mTerrains;
    Tasks mToDo;
    Chunks mChunks;
    NoiseGen mNoise;
    glm::i32vec2 mPreviousCenter;
    float mChunkSize;
    int mViewDistance;
    long mFrameID;
    int mMaxRes;
    int mTaskPerFrame;
};

#endif // WORLD_H
