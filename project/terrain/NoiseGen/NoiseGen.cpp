#include "NoiseGen.h"

NoiseGen::NoiseGen() {

}

NoiseGen::NoiseGen(float size) {
    init(size);
}

void NoiseGen::init(float size) {

    // set screenquad size
    this->mSize = size;

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

void NoiseGen::draw(const glm::mat4& model, float res) const {
    glUseProgram(mProgramId);
    glBindVertexArray(mVertexArrayId);

    glUniform1f(glGetUniformLocation(mProgramId,"res"),res);

    glUniformMatrix4fv(glGetUniformLocation(mProgramId,"M"), ONE, DONT_TRANSPOSE,glm::value_ptr(model));
    // draw
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glBindVertexArray(0);
    glUseProgram(0);
}
