#include "quad.h"

#include "icg_helper.h"

void Quad::Init(const string &texture_filename, const string &vshader, const string &fshader) {
    // compile the shaders
    program_id_ = icg_helper::LoadShaders("quad_vshader.glsl",
                                          "quad_fshader.glsl");

    if(!program_id_) {
        exit(EXIT_FAILURE);
    }

    glUseProgram(program_id_);

    // vertex one vertex Array
    glGenVertexArrays(1, &vertex_array_id_);
    glBindVertexArray(vertex_array_id_);

    // vertex coordinates
    {
        const GLfloat vertex_point[] = { /*V1*/ -0.f, -.0f, 0.0f,
                                         /*V2*/ +1.0f, -.0f, 0.0f,
                                         /*V3*/ -.0f, +1.0f, 0.0f,
                                         /*V4*/ +1.0f, +1.0f, 0.0f};
        // buffer
        glGenBuffers(1, &vertex_buffer_object_);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object_);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_point),
                     vertex_point, GL_STATIC_DRAW);

        // attribute
        GLuint vertex_point_id = glGetAttribLocation(program_id_, "vpoint");
        glEnableVertexAttribArray(vertex_point_id);
        glVertexAttribPointer(vertex_point_id, 3, GL_FLOAT, DONT_NORMALIZE,
                              ZERO_STRIDE, ZERO_BUFFER_OFFSET);
    }

    // texture coordinates
    {
        const GLfloat vertex_texture_coordinates[] = { /*V1*/ 0.0f, 0.0f,
                                                       /*V2*/ 1.0f, 0.0f,
                                                       /*V3*/ 0.0f, 1.0f,
                                                       /*V4*/ 1.0f, 1.0f};

        // buffer
        glGenBuffers(1, &vertex_buffer_object_);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object_);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_texture_coordinates),
                     vertex_texture_coordinates, GL_STATIC_DRAW);

        // attribute
        GLuint vertex_texture_coord_id = glGetAttribLocation(program_id_,
                                                             "vtexcoord");
        glEnableVertexAttribArray(vertex_texture_coord_id);
        glVertexAttribPointer(vertex_texture_coord_id, 2, GL_FLOAT,
                              DONT_NORMALIZE, ZERO_STRIDE,
                              ZERO_BUFFER_OFFSET);
    }

    // load texture
    {
        int width;
        int height;
        int nb_component;
        // set stb_image to have the same coordinates as OpenGL
        stbi_set_flip_vertically_on_load(1);
        unsigned char* image = stbi_load(texture_filename.c_str(), &width,
                                         &height, &nb_component, 0);

        if(image == nullptr) {
            throw(string("Failed to load texture"));
        }

        glGenTextures(1, &texture_id_);
        glBindTexture(GL_TEXTURE_2D, texture_id_);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        if(nb_component == 3) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
                         GL_RGB, GL_UNSIGNED_BYTE, image);
        } else if(nb_component == 4) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
                         GL_RGBA, GL_UNSIGNED_BYTE, image);
        }

        GLuint tex_id = glGetUniformLocation(program_id_, "tex");
        glUniform1i(tex_id, 0 /*GL_TEXTURE0*/);

        // cleanup
        glBindTexture(GL_TEXTURE_2D, 0);
        stbi_image_free(image);
    }

    // Model View Project uniform
    {
        MVP_id_ = glGetUniformLocation(program_id_, "MVP");
    }

    // to avoid the current object being polluted
    glBindVertexArray(0);
    glUseProgram(0);
}

void  Quad::Cleanup() {
    glBindVertexArray(0);
    glUseProgram(0);
    glDeleteBuffers(1, &vertex_buffer_object_);
    glDeleteProgram(program_id_);
    glDeleteVertexArrays(1, &vertex_array_id_);
    glDeleteTextures(1, &texture_id_);
}

void  Quad::Draw(const glm::mat4 &model ,
          const glm::mat4 &view,
          const glm::mat4 &projection) {
    glUseProgram(program_id_);
    glBindVertexArray(vertex_array_id_);

    // bind textures
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_id_);

    // setup MVP
    glm::mat4 MVP = projection*view*model;
    glUniformMatrix4fv(MVP_id_, ONE, DONT_TRANSPOSE, glm::value_ptr(MVP));

    // draw
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glBindVertexArray(0);
    glUseProgram(0);
}
