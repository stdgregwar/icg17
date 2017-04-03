#include "Terrain.h"

void Terrain::init(GLuint textureId) {
    // compile the shaders.
    mHeightMapId = textureId;

    mProgramId = icg_helper::LoadShaders("terrain_vshader.glsl",
                                          "terrain_fshader.glsl");
    if(!mProgramId) {
        exit(EXIT_FAILURE);
    }

    glUseProgram(mProgramId);

    // vertex one vertex array
    glGenVertexArrays(1, &mVertexArrayId);
    glBindVertexArray(mVertexArrayId);

    // vertex coordinates and indices
    {
        std::vector<GLfloat> vertices;
        std::vector<GLuint> indices;

        int gridDim = 1024;

        for(int x = 0; x <= gridDim; ++x)
        {
            for(int y = 0; y <= gridDim; ++y)
            {
                vertices.push_back((float)x/(gridDim/2.f)-1.f); vertices.push_back((float)-y/(gridDim/2.f)+1.f);
            }
        }

        for(int x = 0; x < gridDim; ++x)
        {
            for(int y = 0; y < gridDim; ++y)
            {

                int offset = x * (gridDim+1) + y;
                indices.push_back(offset + 0);
                indices.push_back(offset + 1);
                indices.push_back(offset + gridDim + 1);
                indices.push_back(offset + gridDim + 1);
                indices.push_back(offset + 1);
                indices.push_back(offset + gridDim + 1 + 1);
            }
        }

        mNumIndices = indices.size();

        // position buffer
        glGenBuffers(1, &mVertexBufferObjectPosition);
        glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObjectPosition);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat),
                     &vertices[0], GL_STATIC_DRAW);

        // vertex indices
        glGenBuffers(1, &mVertexBufferObjectIndex);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVertexBufferObjectIndex);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint),
                     &indices[0], GL_STATIC_DRAW);

        // position shader attribute
        GLuint locPosition = glGetAttribLocation(mProgramId, "position");
        glEnableVertexAttribArray(locPosition);
        glVertexAttribPointer(locPosition, 2, GL_FLOAT, DONT_NORMALIZE,
                              ZERO_STRIDE, ZERO_BUFFER_OFFSET);
    }


    // other uniforms
    mMVPId = glGetUniformLocation(mProgramId, "MVP");
    mHeightMapLoc = glGetUniformLocation(mProgramId,"height_map");
    mColorMapLoc = glGetUniformLocation(mProgramId, "color_map");

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mHeightMapId);

    // Color map
    glGenTextures(1, &mColorMapId);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_1D, mColorMapId);
    const int colormapSize=5;
    unsigned char colors[3*colormapSize] = {/*blue*/ 0, 129, 213,
    /*yellow*/ 238, 225, 94,
    /*green*/ 23, 154, 21,
    /*grey*/ 119, 136, 119,
    /*white*/ 249,249,249};
    glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB, colormapSize, 0, GL_RGB, GL_UNSIGNED_BYTE, colors);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf( GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf( GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // to avoid the current object being polluted
    glBindTexture(GL_TEXTURE_1D, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
    glUseProgram(0);
}

void Terrain::cleanup() {
    glBindVertexArray(0);
    glUseProgram(0);
    glDeleteBuffers(1, &mVertexBufferObjectPosition);
    glDeleteBuffers(1, &mVertexBufferObjectIndex);
    glDeleteVertexArrays(1, &mVertexArrayId);
    glDeleteProgram(mProgramId);
    glDeleteTextures(1, &mHeightMapId);
    glDeleteTextures(1, &mColorMapId);
}

void Terrain::draw(float time, const glm::mat4 &model,
          const glm::mat4 &view,
          const glm::mat4 &projection) {
    glUseProgram(mProgramId);
    glBindVertexArray(mVertexArrayId);

    // bind textures

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mHeightMapId);
    glUniform1i(mHeightMapLoc,0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_1D, mColorMapId);
    glUniform1i(mColorMapLoc,GL_TEXTURE1-GL_TEXTURE0);

    // setup MVP
    glm::mat4 MVP = projection*view*model;
    glUniformMatrix4fv(mMVPId, ONE, DONT_TRANSPOSE, glm::value_ptr(MVP));
    glUniformMatrix4fv(glGetUniformLocation(mProgramId,"MV"), ONE, DONT_TRANSPOSE,glm::value_ptr(view*model));

    // pass the current time stamp to the shader.
    glUniform1f(glGetUniformLocation(mProgramId, "time"), time);

    // drawing the grid
    glDrawElements(GL_TRIANGLES, mNumIndices, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    glUseProgram(0);
}
