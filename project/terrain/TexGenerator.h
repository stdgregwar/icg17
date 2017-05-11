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

typedef std::promise<SharedTexture> TexPromise;
typedef std::future<SharedTexture> TexFuture;
typedef std::function<void(ScreenQuad& rd)> RenderFunc;
typedef std::lock_guard<std::mutex> Lock;

class ChunkTexGenerator
{
public:
    struct Job {
        Job(const glm::ivec3& posAndSize)
            : posAndSize(posAndSize), valid(true), promise(TexPromise())
        {}
        TexPromise promise;
        glm::ivec3 posAndSize;
        //RenderFunc render;
        bool valid;
    };
    typedef std::queue<Job> Jobs;

    ChunkTexGenerator(size_t cacheByteSize, float csize);
    SharedTexture prod(const glm::ivec3& k);
    void init(GLFWwindow* parentWindow, const string &vshader, const string &fshader);
    void start();
    void stop();
    /*TexFuture getTexture(const glm::ivec2 size,
                         const RenderFunc& render, Job*& handle);*/
    TexFuture getChunkTex(const glm::ivec3 &posAndSize, float csize, Job*& handle);
    ~ChunkTexGenerator();
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
    rmg::HashCache<glm::ivec3,SharedTexture> mCache;
};

#endif // GLWORKER_H
