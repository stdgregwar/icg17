#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glew/include/GL/glew.h>
#include <string>

class Quad {

    private:
        GLuint vertex_array_id_; // vertex array object
        GLuint program_id_; // GLSL shader program ID
        GLuint vertex_buffer_object_; // memory buffer
        GLuint texture_id_; // texture ID
        GLuint MVP_id_; // MVP matrix

    public:
        void Init(const std::string& texture_filename,
                  const std::string& vshader = "quad_vshader.glsl",
                  const std::string& fshader = "quad_fshader.glsl");
        void Cleanup();
        void Draw(const glm::mat4 &model = glm::mat4(),
                  const glm::mat4 &view =  glm::mat4(),
                  const glm::mat4 &projection =  glm::mat4());
};
