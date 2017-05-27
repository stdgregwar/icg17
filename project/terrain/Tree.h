#ifndef TREE_H
#define TREE_H

#include <glm/vec3.hpp>
#include "Material.h"
#include "Bezier.h"

#include <random>

struct VertexData {
    glm::vec3 pos;
    glm::vec3 normal;
    glm::vec3 col;
    glm::vec2 uv;
};

class Tree
{
public:
    Tree();
    void build(const glm::vec3& pos, const glm::vec3& normal, float width);
    void draw(const glm::mat4& view, const glm::mat4& proj);
    void addTrunc(const Bezier<glm::vec3>& b, float count, float res, float width);
    void addLeaves(const Bezier<glm::vec3>& b, float count);
private:
    std::uniform_real_distribution<float> mRand;
    std::default_random_engine mEng;
    vector<VertexData> mTruncVerts;
    vector<GLuint> mTruncIndices;
    Material mMaterial;
    size_t mTruncSize;
    size_t mLeafSize;
    GLuint mVertexArrayId;
    GLuint mVertexBufferObject;
    GLuint mVertexBufferObjectIndex;
};

#endif // TREE_H
