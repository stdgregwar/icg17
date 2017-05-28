#include "TreePlanes.h"

TreePlanes::TreePlanes()
{

}

void TreePlanes::addTree(const glm::vec3& pos, const glm::vec3 dir) {
    mVertices.push_back({pos,glm::vec4(dir,rand())});
}

void TreePlanes::build() {
    glGenBuffers(1, &mVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(PlanesData),
                 mVertices.data(), GL_STATIC_DRAW);
    mVertexCount = mVertices.size();
    mVertices.clear();
}

void TreePlanes::finish(Material& mat) {

    if(mFinished) return;
    mFinished = true;

    mat.bind();
    glGenVertexArrays(1, &mVertexArrayId);
    glBindVertexArray(mVertexArrayId);

    glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
    {
        // position shader attribute
        GLuint locPosition = mat.attrLocation("position");
        if(locPosition == -1) {
            throw std::runtime_error("failed to bind attrib position");
        }
        glEnableVertexAttribArray(locPosition);
        glVertexAttribPointer(locPosition, 3, GL_FLOAT, DONT_NORMALIZE,
                              sizeof(PlanesData), (void*)offsetof(PlanesData,pos));


        GLuint locDir = mat.attrLocation("dir");
        if(locPosition == -1) {
            throw std::runtime_error("failed to bind attrib dir");
        }
        glEnableVertexAttribArray(locDir);
        glVertexAttribPointer(locDir, 4, GL_FLOAT, DONT_NORMALIZE,
                              sizeof(PlanesData), (void*)offsetof(PlanesData,dir));
    }
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);
    mat.unbind();
}

void TreePlanes::draw(float time, const glm::mat4& view, const glm::mat4& proj, Material& mat) {
    glBindVertexArray(mVertexArrayId);
    glDrawArrays(GL_POINTS, 0, mVertexCount);
    glBindVertexArray(0);
}

TreePlanes::~TreePlanes() {
    if(mFinished) {
        glDeleteBuffers(1,&mVertexBuffer);
        glDeleteVertexArrays(1, &mVertexArrayId);
    }
}
