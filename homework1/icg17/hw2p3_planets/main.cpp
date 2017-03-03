// glew must be before glfw
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// contains helper functions such as shader compiler
#include "icg_helper.h"

#include "quad/quad.h"

using namespace glm;

Quad sun;
Quad earth;
Quad moon;

void Init() {
    // sets background color
    glClearColor(0.1,0.1,0.1 /*white*/, 1.0 /*solid*/);
    glEnable (GL_BLEND); glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    sun.Init("sun.tga");
    earth.Init("earth.tga");
    moon.Init("moon.tga");
}

void Display() {

    glClear(GL_COLOR_BUFFER_BIT);
    float time_s = glfwGetTime();
    float time = time_s*0.1;

    float radius = 6;
    float factor = 0.4;

    mat4 view = scale(mat4(1),vec3(0.1));

    mat4 mat = translate(mat4(1),vec3(1,0,0));
    mat4 msun = glm::rotate(mat,time*20.f,vec3(0,0,1));
    sun.Draw(msun,view);

    mat4 earthPos = translate(mat4(1),vec3(cos(time)*radius,sin(time)*radius*factor,0));
    mat4 mearth = glm::rotate(earthPos,time*365.25f,vec3(0,0,1));
    mearth = scale(mearth,vec3(0.6));
    earth.Draw(mearth,view);

    float moonR = radius*0.3;
    float mtime = time*27.32f;
    mat4 moonPos = translate(earthPos,vec3(cos(mtime)*moonR,sin(mtime)*moonR,1));
    mat4 moonRot = glm::rotate(moonPos,mtime,vec3(0,0,1));
    moonRot = scale(moonRot,vec3(0.3));
    moon.Draw(moonRot,view);

    // compute the transformation matrices
    // {stuff}.Draw({stuff}_modelmatrix);
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
    GLFWwindow* window = glfwCreateWindow(512, 512, "planets", NULL, NULL);
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

    // {stuff}.Cleanup()

    // close OpenGL window and terminate GLFW
    glfwDestroyWindow(window);
    glfwTerminate();
    return EXIT_SUCCESS;
}
