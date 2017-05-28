#ifndef WORLD_H
#define WORLD_H

#include <unordered_map>
#include <glm/gtx/hash.hpp>
#include "Chunk.h"
#include "ScreenQuad/ScreenQuad.h"
#include "Skybox/Skybox.h"
#include "Camera.h"
#include "CameraBezier.h"
#include "CameraFreefly.h"
#include "Bezier.h"
#include "FrameBuffer.h"
#include "GBuffer.h"
#include "Light.h"
#include <list>
#include <functional>
#include "Particles/Particle_generator.h"

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
    void pushForPos(i32vec2 cpos);
    void draw(float time);

    Camera& cam() {return *mCamera;}

    void drawShadows(float time,const glm::mat4& view, const glm::mat4& projection);
    void drawGrass(float time, const glm::mat4& view, const glm::mat4& projection);
    void drawTerrain(float time, const glm::mat4& view, const glm::mat4& projection);
    void drawWater(float time, const glm::mat4& view, const glm::mat4& projection);
    void drawReflexions(float time, const glm::mat4& view, const glm::mat4& projection);

    void tWater() {mRenderWater = !mRenderWater;}
    void tReflexions() {mRenderReflexion = !mRenderReflexion;}
    void tTerrain() {mRenderTerrain = !mRenderTerrain;}
    void tShadows() {mRenderShadow = !mRenderShadow;}
    void tSkybox() {mRenderSkybox = !mRenderSkybox;}
    void tGrass() {mRenderGrass = !mRenderGrass;}

    void registerPoint();
    void tBezierCam();

    void pushTask(ChunkTask task);
    void setScreenSize(const glm::i32vec2& screenSize);
    void stop();
private:
    Camera* mCamera;
    CameraFreefly mCamFreefly;
    CameraBezier mCamBezier;
    BezierBuilder<VecAndDiff> mBezierBuilder;
    Light mLight;
    Material mTerrainMaterial;
    Material mTerrainShadows;
    Material mWaterMaterial;
    Material mGrassMaterial;
    Skybox mSkybox;
    Grids mTerrains;
    Grids mWaters;
    Grids mGrass;
    Tasks mToDo;
    Chunks mChunks;
    ChunkTexGenerator mNoise;
    ScreenQuad mScreen;
    ScreenQuad mRays;
    ScreenQuad mCompositor;
    ScreenQuad mLightPass;
    glm::i32vec2 mCenter;
    glm::i32vec2 mScreenSize;
    GBuffer mGBuffer;
    GBuffer mGMirror;
    ScreenQuad mMirrorLightPass;
    FrameBuffer mMirror;
    FrameBuffer mMain;
    FrameBuffer mFront;
    FrameBuffer mHalf;
    float mChunkSize;
    int mViewDistance;
    long mFrameID;
    int mMaxRes;
    int mTaskPerFrame;
    bool mRenderGrass;
    bool mRenderTerrain;
    bool mRenderReflexion;
    bool mRenderShadow;
    bool mRenderSkybox;
    bool mRenderWater;
    bool mBezierCam;


    ParticleGenerator *mParticles;
};

#endif // WORLD_H
