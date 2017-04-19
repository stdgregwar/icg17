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

typedef std::promise<GLuint> TexPromise;
typedef std::shared_future<GLuint> TexFuture;
typedef std::function<void(ScreenQuad& rd)> RenderFunc;
typedef std::lock_guard<std::mutex> Lock;

class TexGenerator
{
public:
    struct Job {
        Job(const glm::ivec2& size, const RenderFunc& render)
            : size(size), render(render), valid(true), promise(TexPromise())
        {}
        TexPromise promise;
        glm::ivec2 size;
        RenderFunc render;
        bool valid;
    };
    typedef std::queue<Job> Jobs;

    TexGenerator();
    void init(GLFWwindow* parentWindow, const string &vshader, const string &fshader);
    void start();
    void stop();
    TexFuture getTexture(const glm::ivec2 size,
                         const RenderFunc& render, Job*& handle);
    ~TexGenerator();
private:
    void work();
    std::atomic<bool> mContinue;
    std::thread mThread;
    Jobs mJobs;
    std::mutex mJobsMutex;
    GLFWwindow* mWindow;
    string mVShader;
    string mFShader;
    ScreenQuad mGenerator;
};

#endif // GLWORKER_H
