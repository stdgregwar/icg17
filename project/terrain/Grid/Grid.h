#pragma once
#include <glm/gtc/type_ptr.hpp>
#include "icg_helper.h"
#include "../Material.h"

class Grid {
    struct GridVertex{
        glm::vec2 pos;
        GLfloat   shift;
    };

public:
    Grid(Material& m);
    void init(int res, bool seams=true);
    void cleanup();
    void draw(float time, const glm::mat4 &model = IDENTITY_MATRIX,
              const glm::mat4 &view = IDENTITY_MATRIX,
              const glm::mat4 &projection = IDENTITY_MATRIX, GLuint heightMap = 0, GLuint texRes = -1) const;
private:
    Material& mMaterial;
    GLuint mVertexArrayId;                // vertex array object
    GLuint mVertexBufferObjectPosition;  // memory buffer for positions
    GLuint mVertexBufferObjectIndex;     // memory buffer for indices
    GLuint mHeightMapId;                  // texture ID
    GLuint mHeightMapLoc;                 // Uniform location for height map
    GLuint mColorMapLoc;                  // Uniform location for color map
    GLuint mNumIndices;                    // number of vertices to render
    GLuint mMVPId;                         // model, view, proj matrix ID
    int mRes;
};
