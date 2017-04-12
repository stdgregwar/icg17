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

typedef std::promise<GLuint> TexPromise;
typedef std::future<GLuint> TexFuture;
typedef std::function<void(GLuint prog, GLuint fbo)> FboSetup;
typedef std::unordered_map<string,GLuint> Programs;

class TexGenerator
{
    struct Job {
        TexPromise promise;
        GLuint bufferType;
        glm::ivec2 size;
        string shaders;
        FboSetup setup;
    };
    typedef std::queue<Job> Jobs;
public:
    TexGenerator();
    void init(GLFWwindow* parentWindow);
    void start();
    void stop();
    TexFuture getTexture(GLuint bufferType,
                         const glm::ivec2 size,
                         const string& vshader,
                         const string& fshader,
                         const FboSetup& setup);
    GLuint program(const std::string& shader);
private:
    void work();
    std::atomic<bool> mContinue;
    std::thread mThread;
    Jobs mJobs;
    std::mutex mJobsMutex;
    GLFWwindow* mWindow;
    Programs mPrograms;
};

#endif // GLWORKER_H
