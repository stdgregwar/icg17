// glew must be before glfw
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// contains helper functions such as shader compiler
#define STB_IMAGE_IMPLEMENTATION
#define TINYOBJLOADER_IMPLEMENTATION
#include "icg_helper.h"

#include "ScalarFrameBuffer.h"
#include "Camera.h"

#include "Grid/Grid.h"
#include "World.h"

#include <glm/gtc/matrix_transform.hpp>

#ifndef M_PI
#define M_PI 3.1415
#endif

int grid_size = 1024;
int window_width = 1280;
int window_height = 720;
int old_ww;
int old_wh;

#ifndef M_PI
#define M_PI 3.1415
#endif

Camera cam({0,0,150},{-M_PI/4,-M_PI/4,-M_PI/4});

World world(256,cam);

using namespace glm;

void Init(GLFWwindow* window) {
    float ratio = window_width / (float) window_height;
    cam.setProjection(perspective(45.0f, ratio, 0.5f, 10000.0f));
    cam.setBaseSpeed(250);
    glClearColor(1.f, 1.f, 1.f /*white*/, 0.0 /*solid*/);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);


    world.init({window_width,window_height},window);
}

void Display() {
    glViewport(0,0,window_width,window_height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    vec3 cam_pos(5.0f, 5.0f, 5.0f);
    vec3 cam_look(0.0f, 0.0f, 0.0f);
    vec3 cam_up(0.0f, 0.0f, 1.0f);
    mat4 view = cam.view();//lookAt(cam_pos, cam_look, cam_up);
    mat4 view_projection = cam.projection() * view;

    mat4 scalem = scale(mat4(),vec3(4));

    float time = 0.3*glfwGetTime();
    glm::vec2 offset = {time,time};
    offset *= 5.f;
    offset += glm::vec2{-1000.f,231.f};



    world.draw(time,view,cam.projection());
}

// Gets called when the windows/framebuffer is resized.
void resize_callback(GLFWwindow* window, int width, int height) {
    glfwGetFramebufferSize(window, &window_width, &window_height);
    float ratio = window_width / (float) window_height;
    cam.setProjection(perspective(45.0f, ratio, 0.5f, 10000.0f));
    glViewport(0, 0, window_width, window_height);
    world.setScreenSize({width,height});
}

void ErrorCallback(int error, const char* description) {
    fputs(description, stderr);
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    static bool fullscreen = false;
    static int old_ww = window_width;
    static int old_wh = window_height;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    } else if(key == GLFW_KEY_F11 && action == GLFW_RELEASE) {
        // Get the desktop resolution.
        const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());



       int desktopWidth = mode->width;
       int desktopHeight = mode->height;

       fullscreen = !fullscreen;

        if ( fullscreen ) {
            // Set window size for "fullscreen windowed" mode to the desktop resolution.
            old_ww = window_width;
            old_wh = window_height;
            glfwSetWindowSize(window,desktopWidth, desktopHeight);
            // Move window to the upper left corner.
            glfwSetWindowPos(window,0, 0);
        } else {
            // Use start-up values for "windowed" mode.
            glfwSetWindowSize(window,old_ww, old_wh);
            //glfwSetWindowPos(originalPosX, originalPosY);
        }
    }
    if(action == GLFW_RELEASE) {
        switch(key) {
        case GLFW_KEY_F5:
            world.tGrass(); break;
        case GLFW_KEY_F6:
            world.tWater(); break;
        case GLFW_KEY_F7:
            world.tReflexions(); break;
        case GLFW_KEY_F8:
            world.tSkybox(); break;
        case GLFW_KEY_F9:
            world.tTerrain(); break;
        case GLFW_KEY_F10:
            world.tShadows(); break;
        default: break;
        }
    }

    cam.onKey(window,key,scancode,action,mods);
}

void CursorCallback(GLFWwindow* window, double xpos, double ypos) {
    cam.onMouse(window,xpos,ypos);
}

void update(float dt) {
    cam.update(dt);
    world.update(dt,cam.wPos());
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
    GLFWwindow* window = glfwCreateWindow(window_width, window_height, "Procedural Terrain", NULL, NULL);
    if( !window ){
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    /// Link the current context to the window we created
    glfwMakeContextCurrent(window);

    /// Set the callback for escape key
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window,CursorCallback);

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

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    /// Render loop & keyboard input
    glfwSwapInterval(1);
    float lastTime = glfwGetTime();
    while(!glfwWindowShouldClose(window)){
        float time = glfwGetTime();
        update(time-lastTime);
        Display();
        glfwSwapBuffers(window);
        glfwPollEvents();
        lastTime=time;
    }
    world.stop();
    /// Close OpenGL window and terminate GLFW
    glfwDestroyWindow(window);
    glfwTerminate();
    exit( EXIT_SUCCESS );
}
