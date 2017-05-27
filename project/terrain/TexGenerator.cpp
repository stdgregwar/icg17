#include "TexGenerator.h"
#include "ScalarFrameBuffer.h"

#include <glm/gtc/matrix_transform.hpp>

ChunkGenerator::ChunkGenerator(size_t cacheByteSize, float csize, const Grids &terrains, const Grids &waters, const Grids &grass) : mChunkSize(csize),
    mCache(cacheByteSize,[this](const glm::ivec3& k){return prod(k);}),
    mTerrains(terrains),
    mWaters(waters),
    mGrass(grass)
{

}


void ChunkGenerator::init(GLFWwindow* parentWindow,const string& vshader, const string& fshader) {
    mVShader = vshader;
    mFShader = fshader;
    glfwWindowHint(GLFW_VISIBLE, false);
    mWindow = glfwCreateWindow(512, 512, "Second Window",NULL, parentWindow);
    glClampColor(GL_CLAMP_READ_COLOR, GL_FIXED_ONLY);
    start();
}

void ChunkGenerator::start() {
    mContinue = true;
    mThread = std::thread(std::bind(&ChunkGenerator::work,this));
}

void ChunkGenerator::stop() {
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

ChunkGenerator::SharedJob ChunkGenerator::getChunkJob(const glm::ivec3& posAndSize, float csize) {
    using namespace glm;
    Lock l(mJobsMutex);
    mJobs.emplace(new Job(posAndSize));
    mJobs.back()->future = move(mJobs.back()->promise.get_future());
    return mJobs.back();
}

SharedTexture ChunkGenerator::prod(const glm::ivec3& k) {
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
    GCTexture* gct = new GCTexture(tex,k.z,k.z);
    glReadPixels(0,0,k.z,k.z,GL_RED,GL_FLOAT,gct->data());
    fb.unbind();
    glFinish();
    return SharedTexture(gct);
}

void ChunkGenerator::work() {
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
        SharedJob j = mJobs.front();
        mJobsMutex.unlock();
        //if(j.valid) {

        //Create new chunk
        vec2 offset(j->posAndSize.x*mChunkSize,j->posAndSize.y*mChunkSize);
        SharedChunk c(new Chunk(offset,{mChunkSize,mChunkSize}));

        int res = j->posAndSize.z;
        ivec3 tres(j->posAndSize.x,j->posAndSize.y,j->posAndSize.z*8+2);
        SharedTexture tex = mCache.get(tres);
        c->setAttrs(res,tex,mTerrains.at(res),mWaters.at(res),mGrass.at(res));
        //} else {
        //    tex = 0;
        //}
        try{
            j->promise.set_value(c); //Return the texture to the asker
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

 ChunkGenerator::~ChunkGenerator() {
     //stop();
 }
