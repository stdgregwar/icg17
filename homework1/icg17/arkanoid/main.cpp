// glew must be before glfw
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// contains helper functions such as shader compiler

#include "Game.h"
#include <iostream>

using namespace glm;
using namespace std;
Game* game;

void Init() {
    // sets background color
    glClearColor(0.1,0.1,0.1 /*white*/, 1.0 /*solid*/);
    glEnable (GL_BLEND); glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    game = new Game(800,600);
}

void Display() {
    glClear(GL_COLOR_BUFFER_BIT);
    game->draw();
}

void ErrorCallback(int error, const char* description) {
    cerr << string(description) << endl;
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
        game->right();
    }
    if (key == GLFW_KEY_RIGHT && action == GLFW_RELEASE) {
        game->left();
    }
    if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
        game->left();
    }
    if (key == GLFW_KEY_LEFT && action == GLFW_RELEASE) {
        game->right();
    }
    if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE) {
        game->space();
    }
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

int main(int argc, char *argv[]) {
    // GLFW Initialization
    if(!glfwInit()) {
        cerr << "Failed to initialize GLFW\n" << endl;;
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
    GLFWwindow* window = glfwCreateWindow(800, 600, "Arkanoid-Like", NULL, NULL);
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
        game->tick(glfwGetTime());
        Display();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // {stuff}.Cleanup()

    // close OpenGL window and terminate GLFW
    glfwDestroyWindow(window);
    glfwTerminate();
    return EXIT_SUCCESS;
}
