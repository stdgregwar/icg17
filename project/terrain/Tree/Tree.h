#ifndef TREE_H
#define TREE_H

#include <glm/vec3.hpp>
#include "../Material.h"
#include "../Bezier.h"

#include <random>

struct TruncData {
    glm::vec3 pos;
    glm::vec3 normal;
    glm::vec3 col;
    glm::vec2 uv;
};

struct LeafData {
    glm::vec3 pos;
    glm::vec3 col;
    float size;
};

class Tree
{
public:
    Tree(Material& truncMaterial, Material& leafMaterial);
    void build(const glm::vec3& pos, const glm::vec3& normal, float width);
    void finish();
    void drawTrunc();
    void drawLeaves();
    void addTrunc(const Bezier<glm::vec3>& b, float count, float res, float width);
    void addLeaves(const Bezier<glm::vec3>& b, float count, float bsize);
    const glm::vec3& pos() {return mPos;}
    ~Tree();
private:
    bool mFinished = false;
    glm::vec3 mPos;
    std::uniform_real_distribution<float> mRand;
    std::mt19937 mEng;
    vector<TruncData> mTruncVerts;
    vector<GLuint> mTruncIndices;
    vector<LeafData> mLeafVerts;
    Material& mTruncMaterial;
    Material& mLeafMaterial;
    size_t mTruncSize;
    size_t mLeafSize;
    GLuint mVertexArrayId;
    GLuint mVertexBufferObject;
    GLuint mLeafVertexArrayId;
    GLuint mLeafVertexBufferObject;
    GLuint mVertexBufferObjectIndex;
};

#endif // TREE_H
