#include "NoiseGen.h"

void NoiseGen::init(float screenquadWidth, float screenquadHeight) {

    // set screenquad size
    this->mScreenquadWidth = screenquadWidth;
    this->mScreenquadHeight = screenquadHeight;

    // compile the shaders
    mProgramId = icg_helper::LoadShaders("NoiseGen_vshader.glsl",
                                          "NoiseGen_fshader.glsl");
    if(!mProgramId) {
        exit(EXIT_FAILURE);
    }

    glUseProgram(mProgramId);

    // vertex one vertex Array
    glGenVertexArrays(1, &mVertexArrayId);
    glBindVertexArray(mVertexArrayId);

    // vertex coordinates
    {
        const GLfloat vertex_point[] = { /*V1*/ -1.0f, -1.0f, 0.0f,
                                         /*V2*/ +1.0f, -1.0f, 0.0f,
                                         /*V3*/ -1.0f, +1.0f, 0.0f,
                                         /*V4*/ +1.0f, +1.0f, 0.0f};
        // buffer
        glGenBuffers(1, &mVertexBufferObject);
        glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObject);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_point),
                     vertex_point, GL_STATIC_DRAW);

        // attribute
        GLuint vertex_point_id = glGetAttribLocation(mProgramId, "vpoint");
        glEnableVertexAttribArray(vertex_point_id);
        glVertexAttribPointer(vertex_point_id, 3, GL_FLOAT, DONT_NORMALIZE,
                              ZERO_STRIDE, ZERO_BUFFER_OFFSET);
    }

    // texture coordinates
    {
        const GLfloat vertexTextureCoordinates[] = { /*V1*/ 0.0f, 0.0f,
                                                       /*V2*/ 1.0f, 0.0f,
                                                       /*V3*/ 0.0f, 1.0f,
                                                       /*V4*/ 1.0f, 1.0f};

        // buffer
        glGenBuffers(1, &mVertexBufferObject);
        glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObject);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertexTextureCoordinates),
                     vertexTextureCoordinates, GL_STATIC_DRAW);

        // attribute
        GLuint vertexTextureCoordId = glGetAttribLocation(mProgramId,
                                                             "vtexcoord");
        glEnableVertexAttribArray(vertexTextureCoordId);
        glVertexAttribPointer(vertexTextureCoordId, 2, GL_FLOAT,
                              DONT_NORMALIZE, ZERO_STRIDE,
                              ZERO_BUFFER_OFFSET);
    }

    // to avoid the current object being polluted
    glBindVertexArray(0);
    glUseProgram(0);
}

void NoiseGen::cleanup() {
    glBindVertexArray(0);
    glUseProgram(0);
    glDeleteBuffers(1, &mVertexBufferObject);
    glDeleteProgram(mProgramId);
    glDeleteVertexArrays(1, &mVertexArrayId);
}

void NoiseGen::updateSize(int screenquadWidth, int screenquadHeight) {
    this->mScreenquadWidth = screenquadWidth;
    this->mScreenquadHeight = screenquadHeight;
}

void NoiseGen::draw(const glm::vec2& offset) {
    glUseProgram(mProgramId);
    glBindVertexArray(mVertexArrayId);

    // window size uniforms
        glUniform1f(glGetUniformLocation(mProgramId, "screenWidth"),
                    this->mScreenquadWidth);
    glUniform2fv(glGetUniformLocation(mProgramId,"offset"),1,(GLfloat*)&offset);

    // draw
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glBindVertexArray(0);
    glUseProgram(0);
}
