#ifndef WORLD_H
#define WORLD_H

#include <unordered_map>
#include <glm/gtx/hash.hpp>
#include "Chunk.h"
#include "ScreenQuad/ScreenQuad.h"
#include "Skybox/Skybox.h"
#include "FrameBuffer.h"
#include <list>
#include <functional>

typedef std::unordered_map<glm::i32vec2,Chunk> Chunks;
typedef std::unordered_map<int,Grid> Grids;

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
    void init(const glm::i32vec2& screenSize, GLFWwindow *window);
    void setViewDistance(int chunks);
    void update(float dt,const glm::vec2& worldPos);
    void updateChunks();
    void draw(float time,const glm::mat4& view, const glm::mat4& projection);
    void pushTask(ChunkTask task);
    void setScreenSize(const glm::i32vec2& screenSize);
private:
    Material mTerrainMaterial;
    Material mWaterMaterial;
    Material mGrassMaterial;
    Skybox mSkybox;
    Grids mTerrains;
    Grids mWaters;
    Grids mGrass;
    Tasks mToDo;
    Chunks mChunks;
    TexGenerator mNoise;
    ScreenQuad mScreen;
    glm::i32vec2 mCenter;
    glm::i32vec2 mScreenSize;
    FrameBuffer mMirror;
    FrameBuffer mMain;
    float mChunkSize;
    int mViewDistance;
    long mFrameID;
    int mMaxRes;
    int mTaskPerFrame;
};

#endif // WORLD_H
