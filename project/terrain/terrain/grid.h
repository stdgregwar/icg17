#pragma once
#include "icg_helper.h"
#include <glm/gtc/type_ptr.hpp>


class Grid {

    private:
        GLuint vertex_array_id_;                // vertex array object
        GLuint vertex_buffer_object_position_;  // memory buffer for positions
        GLuint vertex_buffer_object_index_;     // memory buffer for indices
        GLuint program_id_;                     // GLSL shader program ID
        GLuint height_map_id_;                  // texture ID
        GLuint height_map_loc_;                 // Uniform location for height map
        GLuint color_map_id_;                   // Color map for the terain
        GLuint color_map_loc_;                  // Uniform location for color map
        GLuint num_indices_;                    // number of vertices to render
        GLuint MVP_id_;                         // model, view, proj matrix ID

    public:
        void Init(GLuint texture_id) {
            // compile the shaders.
            height_map_id_ = texture_id;

            program_id_ = icg_helper::LoadShaders("grid_vshader.glsl",
                                                  "grid_fshader.glsl");
            if(!program_id_) {
                exit(EXIT_FAILURE);
            }

            glUseProgram(program_id_);

            // vertex one vertex array
            glGenVertexArrays(1, &vertex_array_id_);
            glBindVertexArray(vertex_array_id_);

            // vertex coordinates and indices
            {
                std::vector<GLfloat> vertices;
                std::vector<GLuint> indices;
                // TODO 5: make a triangle grid with dimension 100x100.
                // always two subsequent entries in 'vertices' form a 2D vertex position.
                int grid_dim = 512;

                // the given code below are the vertices for a simple quad.
                // your grid should have the same dimension as that quad, i.e.,
                // reach from [-1, -1] to [1, 1].

                for(int x = 0; x <= grid_dim; ++x)
                {
                    for(int y = 0; y <= grid_dim; ++y)
                    {
                        vertices.push_back((float)x/(grid_dim/2.f)-1.f); vertices.push_back((float)-y/(grid_dim/2.f)+1.f);
                    }
                }

                for(int x = 0; x < grid_dim; ++x)
                {
                    for(int y = 0; y < grid_dim; ++y)
                    {

                        int offset = x * (grid_dim+1) + y;
                        indices.push_back(offset + 0);
                        indices.push_back(offset + 1);
                        indices.push_back(offset + grid_dim + 1);
                        indices.push_back(offset + grid_dim + 1);
                        indices.push_back(offset + 1);
                        indices.push_back(offset + grid_dim + 1 + 1);
                    }
                }

                num_indices_ = indices.size();

                // position buffer
                glGenBuffers(1, &vertex_buffer_object_position_);
                glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object_position_);
                glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat),
                             &vertices[0], GL_STATIC_DRAW);

                // vertex indices
                glGenBuffers(1, &vertex_buffer_object_index_);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertex_buffer_object_index_);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint),
                             &indices[0], GL_STATIC_DRAW);

                // position shader attribute
                GLuint loc_position = glGetAttribLocation(program_id_, "position");
                glEnableVertexAttribArray(loc_position);
                glVertexAttribPointer(loc_position, 2, GL_FLOAT, DONT_NORMALIZE,
                                      ZERO_STRIDE, ZERO_BUFFER_OFFSET);
            }


            // other uniforms
            MVP_id_ = glGetUniformLocation(program_id_, "MVP");
            height_map_loc_ = glGetUniformLocation(program_id_,"height_map");
            color_map_loc_ = glGetUniformLocation(program_id_, "color_map");

            // Color map

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, height_map_id_);

            glGenTextures(1, &color_map_id_);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_1D, color_map_id_);
            const int ColormapSize=5;
            unsigned char colors[3*ColormapSize] = {/*blue*/ 0, 129, 213,
            /*yellow*/ 238, 225, 94,
            /*green*/ 23, 154, 21,
            /*grey*/ 119, 136, 119,
            /*white*/ 249,249,249};
            glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB, ColormapSize, 0, GL_RGB, GL_UNSIGNED_BYTE, colors);
            glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameterf( GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameterf( GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


//            glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//            glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//            glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);



            // to avoid the current object being polluted
            glBindTexture(GL_TEXTURE_1D, 0);
            glBindTexture(GL_TEXTURE_2D, 0);
            glBindVertexArray(0);
            glUseProgram(0);
        }

        void Cleanup() {
            glBindVertexArray(0);
            glUseProgram(0);
            glDeleteBuffers(1, &vertex_buffer_object_position_);
            glDeleteBuffers(1, &vertex_buffer_object_index_);
            glDeleteVertexArrays(1, &vertex_array_id_);
            glDeleteProgram(program_id_);
            glDeleteTextures(1, &height_map_id_);
            glDeleteTextures(1, &color_map_id_);
        }

        void Draw(float time, const glm::mat4 &model = IDENTITY_MATRIX,
                  const glm::mat4 &view = IDENTITY_MATRIX,
                  const glm::mat4 &projection = IDENTITY_MATRIX) {
            glUseProgram(program_id_);
            glBindVertexArray(vertex_array_id_);

            // bind textures

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, height_map_id_);
            glUniform1i(height_map_loc_,0);

            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_1D, color_map_id_);
            glUniform1i(color_map_loc_,GL_TEXTURE1-GL_TEXTURE0);
            // TODO complete2
            // Color texture

            // setup MVP
            glm::mat4 MVP = projection*view*model;
            glUniformMatrix4fv(MVP_id_, ONE, DONT_TRANSPOSE, glm::value_ptr(MVP));
            glUniformMatrix4fv(glGetUniformLocation(program_id_,"MV"), ONE, DONT_TRANSPOSE,glm::value_ptr(view*model));

            // pass the current time stamp to the shader.
            glUniform1f(glGetUniformLocation(program_id_, "time"), time);

            // draw
            // TODO 5: for debugging it can be helpful to draw only the wireframe.
            // You can do that by uncommenting the next line.
//            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            // TODO 5: depending on how you set up your vertex index buffer, you
            // might have to change GL_TRIANGLE_STRIP to GL_TRIANGLES.
            glDrawElements(GL_TRIANGLES, num_indices_, GL_UNSIGNED_INT, 0);

            glBindVertexArray(0);
            glUseProgram(0);
        }
};
