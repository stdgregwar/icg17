#include "ScreenQuad.h"

ScreenQuad::ScreenQuad() {

}

ScreenQuad::ScreenQuad(const string &vshader, const string &fshader, float size) {
    init(vshader,fshader,size);
}

void ScreenQuad::init(const string &vshader, const string &fshader, float size) {

    // set screenquad size
    this->mSize = size;

    mMaterial.init(vshader,fshader);

    glGenVertexArrays(1, &mVertexArrayId);
    glBindVertexArray(mVertexArrayId);

    // vertex coordinates
    {
        const GLfloat vertex_point[] = { /*V1*/ -1.0f, -1.0f,
                                         /*V2*/ +1.0f, -1.0f,
                                         /*V3*/ -1.0f, +1.0f,
                                         /*V4*/ +1.0f, +1.0f};
        // buffer
        glGenBuffers(1, &mVertexBufferObject);
        glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObject);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_point),
                     vertex_point, GL_STATIC_DRAW);

        // attribute
        GLuint vertex_point_id = mMaterial.attrLocation("vpoint");
        glEnableVertexAttribArray(vertex_point_id);
        glVertexAttribPointer(vertex_point_id, 2, GL_FLOAT, DONT_NORMALIZE,
                              ZERO_STRIDE, ZERO_BUFFER_OFFSET);
    }

    // to avoid the current object being polluted
    glBindVertexArray(0);
}

void ScreenQuad::cleanup() {
    glBindVertexArray(0);
    glDeleteBuffers(1, &mVertexBufferObject);
    glDeleteVertexArrays(1, &mVertexArrayId);
}

void ScreenQuad::draw(const glm::mat4& model, float res) const {
    mMaterial.bind();
    glBindVertexArray(mVertexArrayId);

    glUniform1f(mMaterial.uniformLocation("res"),res);

    glUniformMatrix4fv(mMaterial.uniformLocation("M"), ONE, DONT_TRANSPOSE,glm::value_ptr(model));
    // draw
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glBindVertexArray(0);
    mMaterial.unbind();
}

void ScreenQuad::draw(const mat4 &view, const mat4& projection) const {
    mMaterial.bind();
    glBindVertexArray(mVertexArrayId);

    glUniformMatrix4fv(mMaterial.uniformLocation("V"), ONE, DONT_TRANSPOSE,glm::value_ptr(view));
    glUniformMatrix4fv(mMaterial.uniformLocation("P"), ONE, DONT_TRANSPOSE,glm::value_ptr(projection));
    glUniformMatrix4fv(mMaterial.uniformLocation("iP"),ONE,DONT_TRANSPOSE,glm::value_ptr(inverse(projection)));
    glUniformMatrix4fv(mMaterial.uniformLocation("iV"),ONE,DONT_TRANSPOSE,glm::value_ptr(inverse(view)));
    // draw
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glBindVertexArray(0);
    mMaterial.unbind();
}
