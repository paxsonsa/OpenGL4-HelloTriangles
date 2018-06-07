/*
 * @Author: Andrew Paxson 
 * @Date: 2018-06-06 23:32:54 
 * @Last Modified by: Andrew Paxson
 * @Last Modified time: 2018-06-07 00:04:01
 */
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <stdio.h>

int main(int argc, char const *argv[])
{
    if (!glfwInit ()) {
        fprintf (stderr, "ERROR: could not start GLFW3\n");
        return 1;
    }
    fprintf(stdout, "INFO: started GLFW3!\n");
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(640, 480, "Hello, Triangle!", nullptr, nullptr);
    if (!window) {
        fprintf(stderr, "ERROR: Failed to Initialize the Window");
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    glewInit();
    
    // Version Info
    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* version = glGetString(GL_VERSION);
    printf("Renderer: %s\n", renderer);
    printf("OpenGL Version: %s\n", version);

    // Tell GL only to draw into pixel is shape is closer to the viewer
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    /*
        Work goes here
    */
   glfwTerminate();
    return 0;
}
