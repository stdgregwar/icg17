#include "World.h"

#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "ShaderBuilder.h"
#include "VecAndDiff.h"
#include "Bezier.h"
#include "CameraBezier.h"

using namespace glm;
using namespace std;

#define Mb *1024*1024

World::World(float chunkSize): mCamBezier({0,0,150},{-M_PI/4,-M_PI/4,-M_PI/4},
{{{{0.f,0.f,300.f},{M_PI,0.f,0.f}},{{0.f,0.f,300.f},{M_PI,0.f,0.f}}}}), mCamFreefly({128,128,0},{-M_PI/4,-M_PI/4,-M_PI/4}), mChunkSize(chunkSize), mViewDistance(16),
    mFrameID(0), mCenter(5000,5000), mMaxRes(64),
    mChunkGenerator(1024 Mb, chunkSize,mTerrains,mWaters,mGrass),
    mLight({3000,3000,2*8192},{1,1,-3},{1,250.f/255,223.f/255},{0.2,0.3,0.3}),
    //mLight({3000,4096,3000},{3,3,-1},{1,0.5,0.25},{0.2,0.3,0.3}),
    mRenderGrass(false),
    mRenderTerrain(true),
    mRenderReflexion(true),
    mRenderShadow(true),
    mRenderSkybox(true),
    mRenderWater(true),
    mBezierCam(false)

{
    mChunks.reserve((mViewDistance*2+1)*(mViewDistance*2+1)+128);
    mCamera = &mCamFreefly;
}

void World::init(const ivec2 &screenSize, GLFWwindow* window) {
    // Terrain material initialization
    mLight.init(2048);
    mTerrainShadows.init("terrain_vshader.glsl","foccluder.glsl");
    mTerrainMaterial.init("terrain_vshader.glsl","terrain_fshader.glsl");
    mGrassMaterial.init("terrain_vshader.glsl","grass_fshader.glsl","grass_gshader.glsl");

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
    {colors.size()/3},"color_map",GL_LINEAR,GL_CLAMP_TO_EDGE, false);
    mGrassMaterial.addTexture(GL_TEXTURE_1D,GL_TEXTURE1,colors.data(),GL_RGB,GL_UNSIGNED_BYTE,
    {colors.size()/3},"color_map",GL_LINEAR,GL_CLAMP_TO_EDGE, false);
    mGrassMaterial.addTexture(GL_TEXTURE2, "grasscolor.png","grass_col",GL_LINEAR_MIPMAP_LINEAR,GL_REPEAT, true);
    mGrassMaterial.addTexture(GL_TEXTURE3, "grasspatch.png", "grasspatch",GL_LINEAR_MIPMAP_LINEAR,GL_REPEAT,true);
    mTerrainMaterial.addTexture(GL_TEXTURE2,"moss-albedo.png","moss_alb",GL_LINEAR_MIPMAP_LINEAR,GL_REPEAT,true);
    mTerrainMaterial.addTexture(GL_TEXTURE9,"moss-normal-inverted.png","moss_nor",GL_LINEAR_MIPMAP_LINEAR,GL_REPEAT,true);
    mTerrainMaterial.addTexture(GL_TEXTURE3,"pebbles.jpg","pebbles_alb",GL_LINEAR_MIPMAP_LINEAR,GL_REPEAT,true);
    mTerrainMaterial.addTexture(GL_TEXTURE10,"pebbles_normalmap.jpg","pebbles_nor",GL_LINEAR_MIPMAP_LINEAR,GL_REPEAT,true);
    mTerrainMaterial.addTexture(GL_TEXTURE4,"sand.jpg","sand",GL_LINEAR_MIPMAP_LINEAR,GL_REPEAT,true);
    mTerrainMaterial.addTexture(GL_TEXTURE5,"rock_sliced_Base_Color.png","rock_alb",GL_LINEAR_MIPMAP_LINEAR,GL_REPEAT,true);
    mTerrainMaterial.addTexture(GL_TEXTURE8,"rock_sliced_Normal.png","rock_nor",GL_LINEAR_MIPMAP_LINEAR,GL_REPEAT,true);
    mTerrainMaterial.addTexture(GL_TEXTURE6,"snow.jpg","snow",GL_LINEAR_MIPMAP_LINEAR,GL_REPEAT,true);
    mTerrainMaterial.addTexture(GL_TEXTURE7,"noise.jpg","noise",GL_LINEAR_MIPMAP_LINEAR,GL_REPEAT, true);



    GLuint skyBoxTex = mSkybox.init();
    mTerrainMaterial.addTexture(GL_TEXTURE_CUBE_MAP,GL_TEXTURE8,skyBoxTex,"skybox");

    setScreenSize(screenSize);

    int res = mMaxRes;
    while(res > 1) {
        mTerrains.emplace(std::piecewise_construct,
                          std::forward_as_tuple(res),
                          std::forward_as_tuple(mTerrainMaterial));
        mTerrains.at(res).init(res);
        mGrass.emplace(std::piecewise_construct,
                       std::forward_as_tuple(res),
                       std::forward_as_tuple(mGrassMaterial));
        mGrass.at(res).init(res*2,false);
        mWaters.emplace(std::piecewise_construct,
                        std::forward_as_tuple(res),
                        std::forward_as_tuple(mWaterMaterial));
        mWaters.at(res).init(1,false);
        res = res >> 1;
    }
    mChunkGenerator.init(window,"NoiseGen_vshader.glsl","NoiseGen_fshader.glsl");
}

void World::setScreenSize(const glm::i32vec2& screenSize) {
    mScreenSize = screenSize;
    GLuint texMirror;
    GLuint depthMiror;
    GLuint texMain;
    GLuint depthMain;

    std::tie(texMirror,depthMiror) = mMirror.init(mScreenSize.x/2,mScreenSize.y/2);
    std::tie(texMain,depthMain) = mMain.init(mScreenSize.x,mScreenSize.y);
    mGBuffer.init(mScreenSize.x,mScreenSize.y);
    mGMirror.init(mScreenSize.x/2,mScreenSize.y/2);
    mFront.init(mScreenSize.x,mScreenSize.y);
    mHalf.init(mScreenSize.x/4,mScreenSize.y/4);

    mScreen.init("vbuffercopy.glsl","fbuffercopy.glsl",0);
    mRays.init("vbuffercopy.glsl","godrays.glsl");
    mCompositor.init("vbuffercopy.glsl","compose.glsl");
    mLightPass.init("vlightpass.glsl","flightpass.glsl");
    mMirrorLightPass.init("vlightpass.glsl","flightpass.glsl");

    // Water material initialization
    mWaterMaterial.init("water_vshader.glsl", "water_fshader.glsl");
    // Frame buffer for mirror effect initialization
    mWaterMaterial.addTexture(GL_TEXTURE_2D,GL_TEXTURE1,texMirror,"mirror");
    mWaterMaterial.addTexture(GL_TEXTURE2,"water_normal2.png","waterNormal",GL_LINEAR_MIPMAP_LINEAR,GL_REPEAT, true);
    mWaterMaterial.addTexture(GL_TEXTURE_2D,GL_TEXTURE3,mFront.diffuse(),"refract_col");
    mWaterMaterial.addTexture(GL_TEXTURE_2D,GL_TEXTURE4,mFront.depth(),"refract_depth");
    //mWaterMaterial.addTexture(GL_TEXTURE_2D,GL_TEXTURE5,mLight.depth(),"shadowmap");
    mLight.addTexture(mWaterMaterial);

    mScreen.material().addTexture(GL_TEXTURE_2D,GL_TEXTURE0,texMain,"buffer_color");
    mScreen.material().addTexture(GL_TEXTURE_2D,GL_TEXTURE1,depthMain,"buffer_depth");

    mRays.material().addTexture(GL_TEXTURE_2D,GL_TEXTURE0,mMain.diffuse(),"buffer_color");
    mRays.material().addTexture(GL_TEXTURE_2D,GL_TEXTURE1,mMain.depth(),"buffer_depth");
    //mRays.material().addTexture(GL_TEXTURE_2D,GL_TEXTURE2,mLight.depth(),"shadowmap");
    mLight.addTexture(mRays.material());

    mLightPass.material().addTexture(GL_TEXTURE_2D,GL_TEXTURE0,mGBuffer.diffuse(),"gcolor");
    mLightPass.material().addTexture(GL_TEXTURE_2D,GL_TEXTURE1,mGBuffer.depth(),"gdepth");
    mLightPass.material().addTexture(GL_TEXTURE_2D,GL_TEXTURE2,mGBuffer.normal(),"gnormal");
    //mLightPass.material().addTexture(GL_TEXTURE_2D,GL_TEXTURE3,mLight.depth(),"shadowmap");
    mLight.addTexture(mLightPass.material());

    mMirrorLightPass.material().addTexture(GL_TEXTURE_2D,GL_TEXTURE0,mGMirror.diffuse(),"gcolor");
    mMirrorLightPass.material().addTexture(GL_TEXTURE_2D,GL_TEXTURE1,mGMirror.depth(),"gdepth");
    mMirrorLightPass.material().addTexture(GL_TEXTURE_2D,GL_TEXTURE2,mGMirror.normal(),"gnormal");
    //mMirrorLightPass.material().addTexture(GL_TEXTURE_2D,GL_TEXTURE3,mLight.depth(),"shadowmap");
    mLight.addTexture(mMirrorLightPass.material());

    mCompositor.material().addTexture(GL_TEXTURE_2D,GL_TEXTURE0,mMain.diffuse(),"diffuse");
    mCompositor.material().addTexture(GL_TEXTURE_2D,GL_TEXTURE1,mHalf.diffuse(),"overlay");
}

void World::update(float dt,const glm::vec2& worldPos) {

    //First update camera
    auto it = mChunks.find(mCenter);
    if(it != mChunks.end()) {
        mCamera->update(dt,*it->second.get());
    }
    mLight.update(dt);

    for(ChunkJobs::iterator it = mCJobs.begin(); it != mCJobs.end();) {
        ChunkGenerator::SharedJob job = *it;
        if(job && job->future.valid()) {
            if(job->future.wait_for(std::chrono::microseconds(1)) == future_status::ready) {
                SharedChunk c = job->future.get();
                mChunks[c->key()] = c;
                it = mCJobs.erase(it);
            } else {
                it++;
            }
        } else {
            it = mCJobs.erase(it);
        }
    }
}

void World::pushForPos(const ivec2& cpos) {
    i32vec2 center = mCenter;

    int maxRes = mMaxRes;

    int dist = std::min(std::max(0,std::max(abs(cpos.x-center.x),abs(cpos.y-center.y))-4),(int)std::log2(mMaxRes)-1);
    int res = maxRes >> dist;
    Chunks::iterator it = mChunks.find(cpos);
    if(it == mChunks.end()) { //Chunk does not exist
        //vec2 worldOffset = vec2(cpos)*mChunkSize;
        ivec3 key(cpos,res);
        mCJobs.push_back(mChunkGenerator.getChunkJob(key,mChunkSize));
    } else {
        SharedChunk c = it->second;
        c->setFrameID(mFrameID);
    }
}

void World::updateChunks() {
    i32vec2 center = mCenter;
    mFrameID++;

    pushForPos(center);
    int x, y, dx, dy;
    x = 0;
    y= 1;
    dx = 1;
    dy = 0;
    for(int radius = 0; radius <= (mViewDistance*2+2)*(mViewDistance*2+2); radius++){
        if( (x == y) || ((x > 0) && (x== -y)) || ((x < 0) && (x == 1-y) )){
            int tmp = dy;
            dy = -dx;
            dx = tmp;
        }

        i32vec2 cpos = {center.x+x,center.y+y};
        pushForPos(cpos);
        x += dx;
        y += dy;
    }

    for(Chunks::iterator it = mChunks.begin() ; it!=mChunks.end(); ){
        if(it->second->frameID() != mFrameID) {
            mChunks.erase(it++);
        }else{
            ++it;
        }
    }
}

void World::drawShadows(float time, const mat4 &view, const mat4 &projection){

    mTerrainShadows.bind();
    for(int i = 0; i < 3; i++) {
        mLight.bind(cam(),i);
        for(Chunks::value_type& p : mChunks) {
            //if(mLight.inFrustum(p.second.pos(),mChunkSize,i)) {
                p.second->drawTerrain(time,mLight.view(i),mLight.proj(i),mTerrainShadows,true);
            //}
        }
    }
    mLight.unbind();
    mTerrainShadows.unbind();
}

void World::drawGrass(float time, const mat4 &view, const mat4 &projection) {
    glDisable(GL_CULL_FACE);
    mGrassMaterial.bind();
    mLight.uniforms(mGrassMaterial);
    for(Chunks::value_type& p : mChunks) {
        if(distance(vec2(p.first),vec2(mCenter)) < 3 && mCamera->inFrustum(p.second->pos(),mChunkSize)) {
            p.second->drawGrass(time,view,projection,mGrassMaterial);
        }
    }
    mGrassMaterial.unbind();
    glEnable(GL_CULL_FACE);
}

void World::drawTerrain(float time, const glm::mat4& view, const glm::mat4& projection) {
    mTerrainMaterial.bind();
    mLight.uniforms(mTerrainMaterial);
    for(Chunks::value_type& p : mChunks) {
        if(mCamera->inFrustum(p.second->pos(),mChunkSize)) {
            p.second->drawTerrain(time,view,projection,mTerrainMaterial);
        }
    }
    mTerrainMaterial.unbind();
}

void World::drawWater(float time, const glm::mat4& view, const glm::mat4& projection) {
    glDisable(GL_CULL_FACE);
    mWaterMaterial.bind();
    mLight.uniforms(mWaterMaterial);
    for(Chunks::value_type& p : mChunks) {
        if(mCamera->inFrustum(p.second->pos(),mChunkSize))
            p.second->drawWater(time,view,projection,mWaterMaterial);
    }
    mWaterMaterial.unbind();
    glEnable(GL_CULL_FACE);
}

void World::drawReflexions(float time, const glm::mat4& view, const glm::mat4& projection) {
    mat4 mirror = scale(view,vec3(1.0,1.0,-1.0));

    mGMirror.bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glFrontFace(GL_CW);


    if(mRenderTerrain) {
        glEnable(GL_CLIP_DISTANCE0);
        mTerrainMaterial.bind();
        for(Chunks::value_type& p : mChunks) {
            if(mCamera->inFrustum(p.second->pos(),mChunkSize)) {
                p.second->drawTerrain(time,mirror,projection,mTerrainMaterial);
            }
        }
        mTerrainMaterial.unbind();
        glDisable(GL_CLIP_DISTANCE0);
    }

    if(mRenderGrass) {
        glDisable(GL_CULL_FACE);
        mGrassMaterial.bind();
        for(Chunks::value_type& p : mChunks) {
            if(distance(vec2(p.first),vec2(mCenter)) < 2.5 && mCamera->inFrustum(p.second->pos(),mChunkSize)) {
                p.second->drawGrass(time,mirror,projection,mGrassMaterial);
            }
        }
        mGrassMaterial.unbind();
        glEnable(GL_CULL_FACE);
    }

    glFrontFace(GL_CCW);
    mGMirror.unbind();

    mMirror.bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glFrontFace(GL_CW);
    if(mRenderSkybox) mSkybox.draw(mirror, projection);
    glFrontFace(GL_CCW);
    mMirrorLightPass.material().bind();
    mLight.uniforms(mMirrorLightPass.material());
    mMirrorLightPass.draw(mirror,projection);
    mMirror.unbind();
}

void World::draw(float time) {
    const mat4 view = mCamera->view();
    const mat4 projection = mCamera->projection();
    if(mRenderShadow) drawShadows(time,view,projection);
    if(mRenderReflexion) drawReflexions(time,view,projection);



    mGBuffer.bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if(mRenderTerrain) drawTerrain(time,view,projection);
    //mTree.draw(view,projection);
    if(mRenderGrass) drawGrass(time, view,projection);
    mGBuffer.unbind();

    //Render lightpass for full scene
    mFront.bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mLightPass.material().bind();
    mLight.uniforms(mLightPass.material());
    mLightPass.draw(view,projection);

    mSkybox.material().bind();
    mLight.uniforms(mSkybox.material());
    if(mRenderSkybox) mSkybox.draw(view, projection);
    mFront.unbind();

    mMain.bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //Copy content of already lit scene
    mFront.blit(mMain);
    //Add water
    if(mRenderWater) drawWater(time,view,projection);
    mMain.unbind();

    //Post effect
    mHalf.bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mRays.material().bind();
    mLight.uniforms(mRays.material());
    mRays.draw(view,projection);

    mHalf.unbind();

    glViewport(0,0,mScreenSize.x,mScreenSize.y);
    mCompositor.draw(view,projection);
}

void World::stop() {
    mChunkGenerator.stop();
}

void World::registerPoint() {
    VecAndDiff v(mCamera->pos(),mCamera->rotation());
    mBezierBuilder.addPoint(v);
}

void World::tBezierCam() {
    mBezierCam = !mBezierCam;
    if(mBezierCam){
        Bezier<VecAndDiff> bezier = mBezierBuilder.build();
        mCamBezier.updateBezier(bezier);
        mCamBezier.setProjection(mCamera->projection());
        mCamera = &mCamBezier;
        mCamera->setPos(bezier.firstPoint().v);
        mCamera->setRotation(bezier.firstPoint().d);
    } else {
        mCamFreefly.setPos(mCamera->pos());
        mCamFreefly.setRotation(mCamera->rotation());
        mCamera = &mCamFreefly;
    }
}
