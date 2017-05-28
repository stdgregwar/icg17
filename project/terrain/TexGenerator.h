#ifndef GLWORKER_H
#define GLWORKER_H

#include <mutex>
#include <thread>
#include <future>
#include <queue>
#include <functional>
#include <atomic>
#include <unordered_map>

#include <icg_helper.h>
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>

#include "ScreenQuad/ScreenQuad.h"
#include "Texture.h"

#include <glm/gtx/hash.hpp>
#include "HashCache.h"

#include "Chunk.h"
#include "Grid/Grid.h"

typedef std::promise<SharedChunk> ChunkPromise;
typedef std::future<SharedChunk> ChunkFuture;
typedef std::lock_guard<std::mutex> Lock;

typedef std::unordered_map<int,Grid> Grids;

class ChunkGenerator
{
public:
    struct Job {
        Job(const glm::ivec3& posAndSize)
            : posAndSize(posAndSize), valid(true), promise(ChunkPromise())
        {}
        ChunkPromise promise;
        ChunkFuture future;
        glm::ivec3 posAndSize;
        bool valid;
    };
    typedef std::shared_ptr<Job> SharedJob;
    typedef std::queue<SharedJob> Jobs;

    ChunkGenerator(size_t cacheByteSize, float csize, const Grids& terrains, const Grids& waters, const Grids& grass, int maxRes, Material& trunc, Material& leaf);
    SharedTexture texProd(const glm::ivec3& k);
    SharedChunk chunkProd(const glm::ivec3& k);
    void init(GLFWwindow* parentWindow, const string &vshader, const string &fshader, int maxRes);
    void start();
    void stop();
    /*TexFuture getTexture(const glm::ivec2 size,
                         const RenderFunc& render, Job*& handle);*/
    SharedJob getChunkJob(const glm::ivec3 &posAndSize);
    ~ChunkGenerator();
private:
    void work();
    float mChunkSize;
    std::atomic<bool> mContinue;
    std::thread mThread;
    Jobs mJobs;
    std::mutex mJobsMutex;
    GLFWwindow* mWindow;
    string mVShader;
    string mFShader;
    ScreenQuad mGenerator;
    rmg::HashCache<glm::ivec3,SharedTexture> mTextureCache;
    rmg::HashCache<glm::ivec3,SharedChunk> mChunkCache;
    const Grids& mTerrains;
    const Grids& mWaters;
    const Grids& mGrass;
    int mMaxRes;
    Material& mTruncMaterial;
    Material& mLeafMaterial;
};

#endif // GLWORKER_H
