/*
 * @Author: Andrew Paxson 
 * @Date: 2018-06-06 23:32:54 
 * @Last Modified by: Andrew Paxson
 * @Last Modified time: 2018-06-07 00:41:04
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

    // Vertex Data
    GLfloat points[] = {
        0.0f, 0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f
    };
    
    // Generate the Vertex Buffer Object and Store the Data
    GLuint vbo = 0;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

    // Generate the Vertex Array Object
    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    // Our Shaders
    // Vertex Shader
    const char* vertexShaderStr =
        "#version 410\n"
        "in vec3 position;"
        "void main () {"
        "  gl_Position = vec4 (position, 1.0);"
        "}";

    // Fragment Shader
    const char* fragmentShaderStr = 
        "#version 410\n"
        "out vec4 colour;\n"
        "void main () {\n"
        "   colour = vec4(0.5, 0.0, 0.5, 1.0);\n"
        "}\n";
    
    // Create and Compile Shaders
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderStr, nullptr);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderStr, nullptr);
    glCompileShader(fragmentShader);

    GLuint shaderProg = glCreateProgram();
    glAttachShader(shaderProg, vertexShader);
    glAttachShader(shaderProg, fragmentShader);
    glLinkProgram(shaderProg);

    
    while(!glfwWindowShouldClose(window)){
        // wipe the drawing buffer
        glClearColor(0.6f, 0.6f, 0.8f, 1.0f);
        // glUseProgram(shaderProg);
        // glBindVertexArray(vao);

        // // Draw 0-3 points from the currently bound VAO with current in-use shader
        // glDrawArrays(GL_TRIANGLES, 0, 3);

        // Update other events like input handling
        glfwPollEvents();

        // Put what we have drawn into the display
        glfwSwapBuffers(window);
    }
    


    glfwTerminate();
    return 0;
}
