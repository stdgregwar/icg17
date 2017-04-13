#include "World.h"

#include <glm/gtc/matrix_transform.hpp>

using namespace glm;
using namespace std;

World::World(float chunkSize) : mChunkSize(chunkSize), mViewDistance(16),
    mFrameID(0), mCenter(5000,5000), mMaxRes(64), mTaskPerFrame(512)
{

}

void World::init(const i32vec2 &screenSize, GLFWwindow* window) {
    // Terrain material initialization
    mTerrainMaterial.init("terrain_vshader.glsl","terrain_fshader.glsl");
//    vector<unsigned char> colors = {/*blue*/ 0, 129, 213,
//                                    /*yellow*/ 238, 225, 94,
//                                    /*green*/ 23, 154, 21,
//                                    /*grey*/ 119, 136, 119,
//                                    /*white*/ 249,249,249};
    vector<unsigned char> colors = {/*blue*/ 0, 0, 255,
                                    /*yellow*/ 0, 255, 255,
                                    /*yellow*/ 0, 255, 255,
                                    /*yellow*/ 0, 255, 255,
                                    /*green*/ 0, 255, 0,
                                    /*green*/ 0, 255, 0,
                                    /*green*/ 0, 255, 0,
                                    /*green*/ 0, 255, 0,
                                    /*green*/ 0, 255, 0,
                                    /*green*/ 0, 255, 0,
                                    /*grey*/ 255, 0, 0};
                                    ///*white*/ 255,255,0};
    mTerrainMaterial.addTexture(GL_TEXTURE_1D,GL_TEXTURE1,colors.data(),GL_RGB,GL_UNSIGNED_BYTE,
    {colors.size()/3},"color_map",GL_LINEAR,GL_CLAMP_TO_EDGE);
    mTerrainMaterial.addTexture(GL_TEXTURE2,"grass.jpg","grass",GL_LINEAR_MIPMAP_LINEAR,GL_REPEAT,true);
    mTerrainMaterial.addTexture(GL_TEXTURE3,"pebbles.jpg","pebbles",GL_LINEAR_MIPMAP_LINEAR,GL_REPEAT,true);
    mTerrainMaterial.addTexture(GL_TEXTURE4,"sand.jpg","sand",GL_LINEAR_MIPMAP_LINEAR,GL_REPEAT,true);
    mTerrainMaterial.addTexture(GL_TEXTURE5,"cliffs.jpg","cliffs",GL_LINEAR_MIPMAP_LINEAR,GL_REPEAT,true);
    mTerrainMaterial.addTexture(GL_TEXTURE6,"snow.jpg","snow",GL_LINEAR_MIPMAP_LINEAR,GL_REPEAT,true);
    mTerrainMaterial.addTexture(GL_TEXTURE7,"noise.jpg","noise");
    mNoise.init(window,"NoiseGen_vshader.glsl","NoiseGen_fshader.glsl");


    setScreenSize(screenSize);

    int res = mMaxRes;
    while(res > 2) {
        mTerrains.emplace(std::piecewise_construct,
                          std::forward_as_tuple(res),
                          std::forward_as_tuple(mTerrainMaterial));
        mTerrains.at(res).init(res);
        mWaters.emplace(std::piecewise_construct,
                          std::forward_as_tuple(res),
                          std::forward_as_tuple(mWaterMaterial));
        mWaters.at(res).init(1,false);
        res = res >> 1;
    }
}

void World::setScreenSize(const glm::i32vec2& screenSize) {
    mScreenSize = screenSize;
    GLuint texMirror;
    GLuint depthMiror;
    GLuint texMain;
    GLuint depthMain;
    std::tie(texMirror,depthMiror) = mMirror.init(mScreenSize.x/2,mScreenSize.y/2);
    std::tie(texMain,depthMain) = mMain.init(mScreenSize.x,mScreenSize.y);
    // Water material initialization
    mWaterMaterial.init("water_vshader.glsl", "water_fshader.glsl");
    // Frame buffer for mirror effect initialization
    mWaterMaterial.addTexture(GL_TEXTURE_2D,GL_TEXTURE1,texMirror,"mirror");
    mWaterMaterial.addTexture(GL_TEXTURE2,"water_normal.png","waterNormal");
    mWaterMaterial.addTexture(GL_TEXTURE_2D,GL_TEXTURE3,texMain,"refract_col");
    mWaterMaterial.addTexture(GL_TEXTURE_2D,GL_TEXTURE4,depthMain,"refract_depth");
    mScreen.init("vbuffercopy.glsl","fbuffercopy.glsl",0);
    mScreen.material().addTexture(GL_TEXTURE_2D,GL_TEXTURE0,texMain,"buffer_color");
    mScreen.material().addTexture(GL_TEXTURE_2D,GL_TEXTURE1,depthMain,"buffer_depth");
}

void World::update(float dt,const glm::vec2& worldPos) {
    i32vec2 center = worldPos/mChunkSize;
    int remaining = mToDo.size();
    //mTaskPerFrame = std::max(8,remaining / 30);
    for(int i = 0; i< mTaskPerFrame && mToDo.size();) {
        ChunkTask& t = mToDo.front();
        switch (t.type) {
        case ChunkTask::CREATE: {
            vec2 worldOffset = vec2(t.chunk)*mChunkSize;
            vec2 size = {mChunkSize,mChunkSize};
            pair<Chunks::iterator,bool> p = mChunks.emplace(std::piecewise_construct,
                                                            std::forward_as_tuple(t.chunk),
                                                            std::forward_as_tuple(worldOffset,size));
            break;
            //i++;
        }
        case ChunkTask::UPDATE:{
            auto it = mChunks.find(t.chunk);
            if(it == mChunks.end()) {
                throw std::runtime_error("Update on null chunk!");
            }
            i+=t.task(&it->second);
            break;
        }
        case ChunkTask::DELETE:
            mChunks.erase(t.chunk);
            //i++;
            break;
        default:
            break;
        }
        mToDo.pop_front();
    }

    if(mCenter != center) {
        mCenter = center;
        updateChunks();
    }

    /*if(mToDo.size()-remaining > mViewDistance*mTaskPerFrame) {
        mTaskPerFrame = std::min(mViewDistance*mViewDistance,mTaskPerFrame+4);
    } else {
        mTaskPerFrame = std::max(4, mTaskPerFrame-1);
    }*/

    for(Chunks::value_type& p : mChunks) {
        p.second.update(dt);
    }

    //cout << "taskspf " << mTaskPerFrame << endl;
}

void World::updateChunks() {
    i32vec2 center = mCenter;
    mFrameID++;
    int maxRes = mMaxRes;
    for(int x = center.x-mViewDistance; x <= center.x+mViewDistance; x++) {
        for(int y = center.y-mViewDistance; y <= center.y+mViewDistance; y++) {
            i32vec2 cpos = {x,y};
            int dist = std::min(std::max(0,std::max(abs(cpos.x-center.x),abs(cpos.y-center.y))-2),4);
            int res = maxRes >> dist;
            Chunks::iterator it = mChunks.find(cpos);
            if(it == mChunks.end()) { //Chunk does not exist
                vec2 worldOffset = vec2(cpos)*mChunkSize;
                vec2 size = {mChunkSize,mChunkSize};
                /*pushTask({
                             ChunkTask::CREATE,
                             cpos,
                             nullptr
                         });*/

                pair<Chunks::iterator,bool> p = mChunks.emplace(std::piecewise_construct,
                                                                std::forward_as_tuple(cpos),
                                                                std::forward_as_tuple(worldOffset,size));
                it = p.first;
            }

            Chunk* c = &it->second;
            c->setFrameID(mFrameID);
            pushTask({
                         ChunkTask::UPDATE,
                         cpos,
                         [this,res](Chunk* c){
                             return c->updateRes(res,mNoise,mTerrains.at(res),mWaters.at(res));
                         }});
            //it->second.update(res,mFrameID,mNoise,mTerrains.at(res));
            //c->updateRes(res,mNoise,mTerrains.at(res),mWaters.at(res));
        }
    }
    for(Chunks::value_type& p : mChunks) {
        if(p.second.frameID() != mFrameID) {
            pushTask({ChunkTask::DELETE,p.second.pos()/mChunkSize,nullptr});
        }
    }
}

void World::pushTask(ChunkTask task) {
    mToDo.remove_if([&task](ChunkTask& ct){return ct.chunk == task.chunk && ct.type != ChunkTask::CREATE;});
    mToDo.push_back(task);
}

void World::draw(float time, const mat4 &view, const mat4 &projection) {
    mat4 mirror = scale(view,vec3(1.0,1.0,-1.0));
    mMirror.bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_CLIP_DISTANCE0);
    glFrontFace(GL_CW);
    mTerrainMaterial.bind();
    for(Chunks::value_type& p : mChunks) {
        if((p.first-mCenter).length() < mViewDistance/4) {
            p.second.drawTerrain(time,mirror,projection);
        }
    }
    mTerrainMaterial.unbind();
    glFrontFace(GL_CCW);
    glDisable(GL_CLIP_DISTANCE0);
    mMirror.unbind();

    mMain.bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    mTerrainMaterial.bind();
    for(auto& p : mChunks) {
        p.second.drawTerrain(time,view,projection);
    }
    mTerrainMaterial.unbind();
    mMain.unbind();

    //mMain.blit(GL_BACK);
    mScreen.draw();

    glViewport(0,0,mScreenSize.x,mScreenSize.y);
    glDisable(GL_CULL_FACE);
    mWaterMaterial.bind();
    for(auto& p : mChunks) {
        p.second.drawWater(time,view,projection);
    }
    mWaterMaterial.unbind();
    glEnable(GL_CULL_FACE);
}
