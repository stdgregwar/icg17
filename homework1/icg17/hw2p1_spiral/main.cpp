// glew must be before glfw
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// contains helper functions such as shader compiler
#include "icg_helper.h"

#include <glm/gtc/matrix_transform.hpp>

#include "triangle/triangle.h"

Triangle triangle;

glm::mat4 tsrMat(float tx, float ty, float scale, float angle);

const bool FERMAT = true;
const float PI = 3.14159;

// Constants for simple spiral
// Controls the number of triangles drawn
const int NUMTRIANGLES_SIMPLE = 60;
// Controls the distance between each triangle
const float B = 0.04;
// Set the max triangle size
const float MAX_TRIANGLE_SIZE = 0.08;
// Set the min triangle size
const float MIN_TRIANGLE_SIZE = 0.01;

// Constants for Fermat's spiral
const float RATIO = 137.508;
// Controls the number of triangles drawn
const int NUMTRIANGLES_FERMAT = 500;
// Controls the scaling factor
const float C = 0.003;
// Controls the triangles size
const float TRIANGLE_SIZE = 0.02;

void Init() {
    // sets background color
    glClearColor(0.937, 0.937, 0.937 /*gray*/, 1.0 /*solid*/);

    triangle.Init();
}

void Display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glm::mat4 model = IDENTITY_MATRIX;
    if(FERMAT){
        for(int i = 1; i <= NUMTRIANGLES_FERMAT; i++) {
            float theta = i * RATIO;
            float r = C*sqrt(theta);
            model = tsrMat(r*cos(theta),r*sin(theta),TRIANGLE_SIZE,theta);
            triangle.Draw(model);
        }
    } else {
        float dTheta = 3*2*PI/NUMTRIANGLES_SIMPLE;
        float dScale = (MAX_TRIANGLE_SIZE-MIN_TRIANGLE_SIZE)/NUMTRIANGLES_SIMPLE;
        for(int i = 0; i <= NUMTRIANGLES_SIMPLE; i++) {
            float theta = i*dTheta;
            float r = B*theta;
            model = tsrMat(r*cos(theta),r*sin(theta),i*dScale+MIN_TRIANGLE_SIZE,dTheta*i);
            triangle.Draw(model);
        }
    }
}

glm::mat4 tsrMat(float tx, float ty, float scale, float angle){
    glm::mat4 model = IDENTITY_MATRIX;
    model = glm::translate(model,glm::vec3(tx,ty,0));
    model = glm::rotate(model,(glm::float32)angle,glm::vec3(0,0,1));
    model = glm::scale(model,glm::vec3(scale,scale,0));
    return model;
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
    // GLFW Initialization
    if(!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return EXIT_FAILURE;
    }

    glfwSetErrorCallback(ErrorCallback);

    // hint GLFW that we would like an OpenGL 3 context (at least)
    // http://www.glfw.org/faq.html#how-do-i-create-an-opengl-30-context
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // attempt to open the window: fails if required version unavailable
    // note some Intel GPUs do not support OpenGL 3.2
    // note update the driver of your graphic card
    GLFWwindow* window = glfwCreateWindow(512, 512, "spiral", NULL, NULL);
    if(!window) {
        glfwTerminate();
        return EXIT_FAILURE;
    }

    // makes the OpenGL context of window current on the calling thread
    glfwMakeContextCurrent(window);

    // set the callback for escape key
    glfwSetKeyCallback(window, KeyCallback);

    // GLEW Initialization (must have a context)
    // https://www.opengl.org/wiki/OpenGL_Loading_Library
    glewExperimental = GL_TRUE; // fixes glew error (see above link)
    if(glewInit() != GLEW_NO_ERROR) {
        fprintf( stderr, "Failed to initialize GLEW\n");
        return EXIT_FAILURE;
    }

    cout << "OpenGL" << glGetString(GL_VERSION) << endl;

    // initialize our OpenGL program
    Init();

    // render loop
    while(!glfwWindowShouldClose(window)) {
        Display();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    triangle.Cleanup();

    // close OpenGL window and terminate GLFW
    glfwDestroyWindow(window);
    glfwTerminate();
    return EXIT_SUCCESS;
}

