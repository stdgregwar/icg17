#pragma once
#include <glm/gtc/type_ptr.hpp>
#include "icg_helper.h"

class Terrain {

    struct TerrainVertex{
        glm::vec2 pos;
        GLfloat   shift;
    };

    private:
        GLuint mVertexArrayId;                // vertex array object
        GLuint mVertexBufferObjectPosition;  // memory buffer for positions
        GLuint mVertexBufferObjectIndex;     // memory buffer for indices
        GLuint mProgramId;                     // GLSL shader program ID
        GLuint mHeightMapId;                  // texture ID
        GLuint mHeightMapLoc;                 // Uniform location for height map
        GLuint mColorMapId;                   // Color map for the terain
        GLuint mColorMapLoc;                  // Uniform location for color map
        GLuint mNumIndices;                    // number of vertices to render
        GLuint mMVPId;                         // model, view, proj matrix ID

    public:
        void init(int res);
        void cleanup();
        void draw(float time, const glm::mat4 &model = IDENTITY_MATRIX,
                  const glm::mat4 &view = IDENTITY_MATRIX,
                  const glm::mat4 &projection = IDENTITY_MATRIX, GLuint heightMap = 0) const;
};
