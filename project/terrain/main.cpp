// glew must be before glfw
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// contains helper functions such as shader compiler
#define STB_IMAGE_IMPLEMENTATION
#define TINYOBJLOADER_IMPLEMENTATION
#include "icg_helper.h"

#include "ScalarFrameBuffer.h"
#include "NoiseGen/NoiseGen.h"

#include "Terrain/Terrain.h"

#include <glm/gtc/matrix_transform.hpp>

int grid_size = 512;
int window_width = 1280;
int window_height = 720;

ScalarFrameBuffer framebuffer;
NoiseGen noiseGen;
Terrain terrain;

using namespace glm;

mat4 projection_matrix;

void Init(GLFWwindow* window) {
    glClearColor(0.0, 0.0, 0.0 /*white*/, 1.0 /*solid*/);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);

    float ratio = window_width / (float) window_height;
    projection_matrix = perspective(45.0f, ratio, 0.1f, 10000.0f);
    GLuint hmap = framebuffer.init(grid_size,grid_size);
    noiseGen.init(window_width,window_height);
    terrain.init(hmap);
}

void Display() {
    glViewport(0,0,window_width,window_height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    vec3 cam_pos(5.0f, 5.0f, 5.0f);
    vec3 cam_look(0.0f, 0.0f, 0.0f);
    vec3 cam_up(0.0f, 0.0f, 1.0f);
    mat4 view = lookAt(cam_pos, cam_look, cam_up);
    mat4 view_projection = projection_matrix * view;

    mat4 scalem = scale(mat4(),vec3(4));

    float time = 0.3*glfwGetTime();
    glm::vec2 offset = {time,time};
//    offset *= 2.f;
    offset += glm::vec2{-1000.f,231.f};

    framebuffer.bind();
    glViewport(0,0,framebuffer.width,framebuffer.height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //cube.Draw(mirror);
    noiseGen.draw(-offset);
    framebuffer.unbind();

    glViewport(0,0,window_width,window_height);
    terrain.draw(time,scalem,view,projection_matrix);
}

// Gets called when the windows/framebuffer is resized.
void resize_callback(GLFWwindow* window, int width, int height) {
    glfwGetFramebufferSize(window, &window_width, &window_height);
    float ratio = window_width / (float) window_height;
    projection_matrix = perspective(45.0f, ratio, 0.1f, 1000.0f);
    glViewport(0, 0, window_width, window_height);
    framebuffer.cleanup();
    terrain.init(framebuffer.init(grid_size,grid_size));
}

void ErrorCallback(int error, const char* description) {
    fputs(description, stderr);
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

int main(int argc, char *argv[]) {
    /// GLFW Initialization
    if( !glfwInit() ){
        fprintf( stderr, "Failed to initialize GLFW\n" );
        return EXIT_FAILURE;
    }

    glfwSetErrorCallback(ErrorCallback);

    /// Hint GLFW that we would like an OpenGL 3 context (at least)
    /// http://www.glfw.org/faq.html#how-do-i-create-an-opengl-30-context
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /// Attempt to open the window: fails if required version unavailable
    /// @note some Intel GPUs do not support OpenGL 3.2
    /// @note update the driver of your graphic card
    GLFWwindow* window = glfwCreateWindow(window_width, window_height, "mirror_floor", NULL, NULL);
    if( !window ){
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    /// Link the current context to the window we created
    glfwMakeContextCurrent(window);

    /// Set the callback for escape key
    glfwSetKeyCallback(window, KeyCallback);

    glfwSetFramebufferSizeCallback(window, resize_callback);

    /// GLEW Initialization (must have a context)
    glewExperimental = GL_TRUE; ///<
    if( glewInit() != GLEW_NO_ERROR ){
        fprintf( stderr, "Failed to initialize GLEW\n");
        exit(EXIT_FAILURE);
    }

    std::cout << "OpenGL" << glGetString(GL_VERSION) << std::endl;
    Init(window);
    KeyCallback(window, GLFW_KEY_KP_1, 0, 0, 0);

    /// Render loop & keyboard input
    while(!glfwWindowShouldClose(window)){
        Display();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    /// Close OpenGL window and terminate GLFW
    glfwDestroyWindow(window);
    glfwTerminate();
    exit( EXIT_SUCCESS );
}
