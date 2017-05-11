#include "TexGenerator.h"
#include "ScalarFrameBuffer.h"

#include <glm/gtc/matrix_transform.hpp>

ChunkTexGenerator::ChunkTexGenerator(size_t cacheByteSize, float csize) : mChunkSize(csize),
    mCache(cacheByteSize,[this](const glm::ivec3& k){return prod(k);})
{

}


void ChunkTexGenerator::init(GLFWwindow* parentWindow,const string& vshader, const string& fshader) {
    mVShader = vshader;
    mFShader = fshader;
    glfwWindowHint(GLFW_VISIBLE, false);
    mWindow = glfwCreateWindow(512, 512, "Second Window",NULL, parentWindow);
    start();
}

void ChunkTexGenerator::start() {
    mContinue = true;
    mThread = std::thread(std::bind(&ChunkTexGenerator::work,this));
}

void ChunkTexGenerator::stop() {
    mContinue = false;
    mThread.join();
}

/*TexFuture ChunkTexGenerator::getTexture(
                     const glm::ivec2 size,
                     const RenderFunc& render,
                     Job*& handle) {
    Lock l(mJobsMutex);
    mJobs.emplace(size,render);
    handle = &mJobs.back();
    return mJobs.back().promise.get_future();
}*/

TexFuture ChunkTexGenerator::getChunkTex(const glm::ivec3& posAndSize, float csize, Job*& handle) {
    using namespace glm;
    Lock l(mJobsMutex);
    mJobs.emplace(posAndSize);
    handle = &mJobs.back();
    return mJobs.back().promise.get_future();
}

SharedTexture ChunkTexGenerator::prod(const glm::ivec3& k) {
    using namespace glm;
    GLuint tex;
    //if(j.valid) {
    ScalarFrameBuffer fb;
    tex = fb.init(k.z,k.z);
    fb.bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //j.render(mGenerator);
    vec2 pos(k.x*mChunkSize,k.y*mChunkSize);
    mat4 model(scale(translate(mat4(),vec3(pos,0)),vec3(mChunkSize)));
    mGenerator.draw(model,k.z);
    fb.unbind();
    glFinish();
    return SharedTexture(tex);
}

void ChunkTexGenerator::work() {
    using namespace glm;
    glfwMakeContextCurrent(mWindow);
    mGenerator.init(mVShader,mFShader);
    while(mContinue) {
        if(!mJobs.size()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            continue;
        }
        //cout << "Have " << mJobs.size() << " tex to gen" << endl;
        mJobsMutex.lock();
        Job& j = mJobs.front();
        mJobsMutex.unlock();
        //if(j.valid) {
        SharedTexture tex = mCache.get(j.posAndSize);

        //} else {
        //    tex = 0;
        //}
        try{
            j.promise.set_value(tex); //Return the texture to the asker
        } catch(std::future_error e) {
            cerr << e.what() << endl;
        }
        { //Lock and pop job
            Lock l(mJobsMutex);
            mJobs.pop();
        }
        //std::this_thread::sleep_for(std::chrono::milliseconds(max(1,int(100-mJobs.size()))));
    }
}

 ChunkTexGenerator::~ChunkTexGenerator() {
     //stop();
 }
