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
    void drawTrunc(const glm::mat4& view, const glm::mat4& proj, Material& mat);
    void drawLeaves(const glm::mat4& view, const glm::mat4& proj, Material& mat);
    void addTrunc(const Bezier<glm::vec3>& b, float count, float res, float width);
    void addLeaves(const Bezier<glm::vec3>& b, float count, float bsize);
    ~Tree();
private:
    bool mFinished = false;
    std::uniform_real_distribution<float> mRand;
    std::default_random_engine mEng;
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
