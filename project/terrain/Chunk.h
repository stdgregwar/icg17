#ifndef CHUNK_H
#define CHUNK_H

#include <glm/vec2.hpp>
#include "Grid/Grid.h"
#include "ScalarFrameBuffer.h"

#include "ScreenQuad/ScreenQuad.h"
#include "TexGenerator.h"

class Chunk
{
public:
    Chunk(const glm::vec2& offset, const glm::vec2& size);
    void update(float delta_s);
    int updateRes(int res, TexGenerator &texGen, const Grid& terrain, const Grid& water);
    void drawTerrain(float time, const glm::mat4 &view, const glm::mat4 &projection);
    void drawWater(float time, const glm::mat4 &view, const glm::mat4 &projection);
    void setFrameID(long id);
    long frameID() { return mFrameId;}
    const glm::vec2& pos() const {return mOffset;}
    ~Chunk();
private:
    bool mReady;
    GLuint mHmap;
    ScalarFrameBuffer mNoiseBuffer;
    const Grid* mTerrain;
    const Grid* mWater;
    long mFrameId;
    int mRes;
    glm::vec2 mOffset;
    glm::vec2 mSize;

    TexGenerator::Job* mTexJob;
    TexFuture mTexFuture;
    int mNextRes;
    const Grid* mNextTerrain;
    const Grid* mNextWater;

};

#endif // CHUNK_H
