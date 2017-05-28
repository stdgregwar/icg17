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
#include "Tree/Tree.h"
#include "TexGenerator.h"
#include "Tree/PhotoMaton.h"

typedef std::unordered_map<glm::ivec2,SharedChunk> Chunks;
typedef std::vector<Chunk*> ChunkPtrs;

typedef std::list<ChunkGenerator::SharedJob> ChunkJobs;

class World
{
public:
    World(float chunkSize);
    void init(const glm::ivec2& screenSize, GLFWwindow *window);
    void setViewDistance(int chunks);
    void update(float dt,const glm::vec2& worldPos);
    void updateChunks();
    void pushForPos(const ivec2& cpos);
    void draw(float time);

    void pushTask(const glm::ivec3& task);

    Camera& cam() {return *mCamera;}

    void drawShadows(float time,const glm::mat4& view, const glm::mat4& projection);
    void drawGrass(float time, const glm::mat4& view, const glm::mat4& projection);
    void drawTerrain(float time, const glm::mat4& view, const glm::mat4& projection);
    void drawWater(float time, const glm::mat4& view, const glm::mat4& projection);
    void drawReflexions(float time, const glm::mat4& view, const glm::mat4& projection);
    void drawTrees(float time, const glm::mat4& view, const glm::mat4& projection);

    void tWater() {mRenderWater = !mRenderWater;}
    void tReflexions() {mRenderReflexion = !mRenderReflexion;}
    void tTerrain() {mRenderTerrain = !mRenderTerrain;}
    void tShadows() {mRenderShadow = !mRenderShadow;}
    void tSkybox() {mRenderSkybox = !mRenderSkybox;}
    void tGrass() {mRenderGrass = !mRenderGrass;}

    void registerPoint();
    void tBezierCam();

    void setScreenSize(const glm::i32vec2& screenSize);
    void stop();

    void collectDrawable();
private:
    Camera* mCamera;
    PhotoMaton mPhotoMaton;
    CameraFreefly mCamFreefly;
    CameraBezier mCamBezier;
    BezierBuilder<VecAndDiff> mBezierBuilder;
    Light mLight;
    Material mTerrainMaterial;
    Material mTerrainShadows;
    Material mWaterMaterial;
    Material mGrassMaterial;
    Material mTruncMaterial;
    Material mTruncShadow;
    Material mLeafShadow;
    Material mLeafMaterial;
    Material mPlanesMaterial;
    Material mPlanesShadow;
    Skybox mSkybox;
    Grids mTerrains;
    Grids mWaters;
    Grids mGrass;
    Chunks mChunks;
    ChunkPtrs mToDraw;
    ChunkJobs mCJobs;
    ChunkGenerator mChunkGenerator;
    ScreenQuad mScreen;
    ScreenQuad mRays;
    ScreenQuad mCompositor;
    ScreenQuad mLightPass;
    std::list<glm::ivec3> mTasks;
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
    float mTreeClip;
    int mViewDistance;
    long mFrameID;
    int mMaxRes;
    bool mRenderGrass;
    bool mRenderTerrain;
    bool mRenderReflexion;
    bool mRenderShadow;
    bool mRenderSkybox;
    bool mRenderWater;
    bool mBezierCam;
};

#endif // WORLD_H
