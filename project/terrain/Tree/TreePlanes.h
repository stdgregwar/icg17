#ifndef TREEPLANES_H
#define TREEPLANES_H

#include <icg_helper.h>
#include <glm/vec3.hpp>
#include "../Material.h"

struct PlanesData {
    glm::vec3 pos;
    glm::vec4 dir;
};

class TreePlanes
{
public:
    TreePlanes();
    void addTree(const glm::vec3& pos, const glm::vec3 dir);
    void build();
    void finish(Material& mat);
    void draw(float time, const glm::mat4& view, const glm::mat4& proj, Material& mat);
    ~TreePlanes();
private:
    bool mFinished = false;
    std::vector<PlanesData> mVertices;
    GLuint mVertexArrayId;
    GLuint mVertexBuffer;
    size_t mVertexCount;
};

#endif // TREEPLANES_H
