#include "TexGenerator.h"
#include "ScalarFrameBuffer.h"

TexGenerator::TexGenerator()
{

}


void TexGenerator::init(GLFWwindow* parentWindow,const string& vshader, const string& fshader) {
    mVShader = vshader;
    mFShader = fshader;
    glfwWindowHint(GLFW_VISIBLE, false);
    mWindow = glfwCreateWindow(512, 512, "Second Window",NULL, parentWindow);
    start();
}

void TexGenerator::start() {
    mContinue = true;
    mThread = std::thread(std::bind(&TexGenerator::work,this));
}

void TexGenerator::stop() {
    mContinue = false;
    mThread.join();
}

TexFuture TexGenerator::getTexture(
                     const glm::ivec2 size,
                     const RenderFunc& render,
                     Job*& handle) {
    Lock l(mJobsMutex);
    mJobs.emplace(size,render);
    handle = &mJobs.back();
    return mJobs.back().promise.get_future();
}


void TexGenerator::work() {
    glfwMakeContextCurrent(mWindow);
    mGenerator.init(mVShader,mFShader);
    while(mContinue) {
        if(!mJobs.size()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(33));
            continue;
        }
        mJobsMutex.lock();
        Job& j = mJobs.front();
        mJobsMutex.unlock();
        GLuint tex;
        //if(j.valid) {
            ScalarFrameBuffer fb;
            tex = fb.init(j.size.x,j.size.y);
            fb.bind();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            j.render(mGenerator);
            fb.unbind();
            glFinish(); //Ensure texture is fully loaded
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
    }
}

 TexGenerator::~TexGenerator() {
     stop();
 }
