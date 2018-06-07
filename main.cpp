/*
 * @Author: Andrew Paxson 
 * @Date: 2018-06-06 23:32:54 
 * @Last Modified by: Andrew Paxson
 * @Last Modified time: 2018-06-07 00:59:10
 */
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <stdio.h>

int main(int argc, char const *argv[])
{
    GLFWwindow *window = nullptr;
    const GLubyte* renderer;
    const GLubyte* version;
    GLuint vao, vbo;
    GLfloat points[] = {
        0.0f, 0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f
    };

    /* these are the strings of code for the shaders
	the vertex shader positions each vertex point */
    const char *vertexShaderStr = "#version 410\n"
        "in vec3 pos;"
        "void main () {"
        "   gl_Position = vec4 (pos, 1.0);"
        "}";

    // Fragment Shader
    const char *fragmentShaderStr = "#version 410\n"
        "out vec4 colour;"
        "void main () {"
        "   colour = vec4(0.5, 0.0, 0.5, 1.0);"
        "}";

    /* GL shader objects for vertex and fragment shader [components] */
	GLuint vertexShader, fragmentShader;
	/* GL shader programme object [combined, to link] */
	GLuint shaderProgam;


    if (!glfwInit ()) {
        fprintf (stderr, "ERROR: could not start GLFW3\n");
        return 1;
    }
    fprintf(stdout, "INFO: started GLFW3!\n");
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(640, 480, "Hello, Triangle!", nullptr, nullptr);
    if (!window) {
        fprintf(stderr, "ERROR: Failed to Initialize the Window");
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    glewInit();
    
    // Version Info
    renderer = glGetString(GL_RENDERER);
    version = glGetString(GL_VERSION);
    printf("Renderer: %s\n", renderer);
    printf("OpenGL Version: %s\n", version);

    // Tell GL only to draw into pixel is shape is closer to the viewer
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    /*
        Work goes here
    */
    // Generate the Vertex Buffer Object and Store the Data
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

    // Generate the Vertex Array Object
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    
    // Create and Compile Shaders
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderStr, nullptr);
    glCompileShader(vertexShader);
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderStr, nullptr);
    glCompileShader(fragmentShader);

    shaderProgam = glCreateProgram();
    glAttachShader(shaderProgam, fragmentShader);
    glAttachShader(shaderProgam, vertexShader);
    glLinkProgram(shaderProgam);

    
    while(!glfwWindowShouldClose(window)){
        // wipe the drawing buffer
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glUseProgram(shaderProgam);
        glBindVertexArray(vao);

        // // Draw 0-3 points from the currently bound VAO with current in-use shader
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Update other events like input handling
        glfwPollEvents();

        // Put what we have drawn into the display
        glfwSwapBuffers(window);
    }
    


    glfwTerminate();
    return 0;
}
