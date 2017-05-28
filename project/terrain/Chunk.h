#ifndef CHUNK_H
#define CHUNK_H

#include <glm/vec2.hpp>

#include <memory>

#include "Grid/Grid.h"
#include "ScalarFrameBuffer.h"

#include "ScreenQuad/ScreenQuad.h"
#include "Texture.h"
#include "Tree/Tree.h"
#include "Tree/SimplexNoise.h"
#include "Tree/TreePlanes.h"

class Chunk;

typedef std::shared_ptr<Chunk> SharedChunk;

class Chunk
{
public:
    Chunk(const glm::vec2& offset, const glm::vec2& size);
    void update(float delta_s);
    void finish(Material& treePlanesMat);
    int setAttrs(int res, SharedTexture hmap, const Grid& terrain, const Grid& water, const Grid& grass);
    void addTrees(Material& trunc, Material& leaves, SimplexNoise &n, bool planeOnly = false);
    void drawTerrain(float time, const glm::mat4 &view, const glm::mat4 &projection, Material& mat, bool shad = false);
    void drawGrass(float time, const glm::mat4 &view, const glm::mat4 &projection,Material& mat);
    void drawWater(float time, const glm::mat4 &view, const glm::mat4 &projection,Material& mat);
    void drawTruncs(const glm::vec3& eye, float dist);
    void drawLeaves(const vec3 &eye, float dist);
    void drawPlanes();
    void setFrameID(long id);
    glm::ivec2 key() const;
    long frameID() { return mFrameId;}
    const SharedTexture& hMap() const {return mHmap;}
    const glm::vec2& pos() const {return mOffset;}
    const glm::vec2 cpos() const {return mOffset/mSize;}
    const glm::vec2& size() const {return mSize;}
    int res() const {return mRes;}
    ~Chunk();
private:
    std::uniform_real_distribution<float> mRand;
    std::mt19937 mEng;
    std::vector<Tree> mTrees;
    TreePlanes mTreePlanes;

    SharedTexture mHmap;
    SharedTexture mNextHmap;
    ScalarFrameBuffer mNoiseBuffer;
    const Grid* mTerrain;
    const Grid* mWater;
    const Grid* mGrass;
    long mFrameId;
    int mRes;
    glm::vec2 mOffset;
    glm::vec2 mSize;
    glm::mat4 mModel;

    //ChunkGenerator::Job* mTexJob;
    //ChunkFuture mTexFuture;
};

#endif // CHUNK_H
