/*
 * @Author: Andrew Paxson 
 * @Date: 2018-06-06 23:32:54 
 * @Last Modified by: Andrew Paxson
 * @Last Modified time: 2018-06-07 01:01:25
 */
#include <cstdarg>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <time.h>
#define GL_LOG_FILE "gl.log"

#include "GL/glew.h"
#include "GLFW/glfw3.h"

// Global Timer Code
static double previous_seconds;

// Window Information
int gWindowWidth = 640, gWindowHeight = 480;
int gFbufferWidth = 640, gFbufferHeight = 480;


bool restartGlLog() {
    FILE* file = fopen(GL_LOG_FILE, "a");
    if (!file) {
        fprintf(stderr, "ERROR: could not open GL_LOG_FILE log file %s for writing\n", GL_LOG_FILE);
        return false;
    }
    time_t now = time(nullptr);
    char* date = ctime(&now);
    fprintf(file, "GL_LOG_FILE log. Local Time: %s\n", date);
    fclose(file);
    return true;
}

bool glLogInfo(const char* message, ...) {
    va_list argptr;
    FILE* file = fopen(GL_LOG_FILE, "a");
    if (!file) {
        fprintf(
            stderr, 
            "ERROR: could not open GL_LOG_FILE log file %s for appending\n", 
            GL_LOG_FILE
        );
        return false;
    }

    va_start(argptr, message);
    vfprintf(file, message, argptr);
    va_end(argptr);
    fclose(file);
    return true;
}

bool glLogErr(const char* message, ...) {
    va_list argptr;
    FILE* file = fopen(GL_LOG_FILE, "a");
    if (!file) {
        fprintf(
            stderr, 
            "ERROR: could not open GL_LOG_FILE log file %s for appending", 
            GL_LOG_FILE
        );
        return false;
    }
    va_start(argptr, message);
    vfprintf(file, message, argptr);
    va_end(argptr);
    va_start(argptr, message);
    vfprintf(stderr, message, argptr);
    va_end(argptr);
    fclose(file);
    return true;
}

void logGLParms() {
    GLenum params[] = {
		GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS,
		GL_MAX_CUBE_MAP_TEXTURE_SIZE,
		GL_MAX_DRAW_BUFFERS,
		GL_MAX_FRAGMENT_UNIFORM_COMPONENTS,
		GL_MAX_TEXTURE_IMAGE_UNITS,
		GL_MAX_TEXTURE_SIZE,
		GL_MAX_VARYING_FLOATS,
		GL_MAX_VERTEX_ATTRIBS,
		GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS,
		GL_MAX_VERTEX_UNIFORM_COMPONENTS,
		GL_MAX_VIEWPORT_DIMS,
		GL_STEREO,
	};
	const char *names[] = {
		"GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS",
		"GL_MAX_CUBE_MAP_TEXTURE_SIZE",
		"GL_MAX_DRAW_BUFFERS",
		"GL_MAX_FRAGMENT_UNIFORM_COMPONENTS",
		"GL_MAX_TEXTURE_IMAGE_UNITS",
		"GL_MAX_TEXTURE_SIZE",
		"GL_MAX_VARYING_FLOATS",
		"GL_MAX_VERTEX_ATTRIBS",
		"GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS",
		"GL_MAX_VERTEX_UNIFORM_COMPONENTS",
		"GL_MAX_VIEWPORT_DIMS",
		"GL_STEREO",
    };
    glLogInfo( "GL Context Params:\n" );
    for (int i = 0; i < 10; i++) {
        int v = 0;
        glGetIntegerv(params[i], &v);
        glLogInfo("%s: %i\n", names[i], v);
    }

    int v[2] = {0, 0};
    glGetIntegerv(params[10], v);
    glLogInfo( "%s %i %i\n", names[10], v[0], v[1] );
    unsigned char s = 0;
	glGetBooleanv( params[11], &s );
	glLogInfo( "%s %i\n", names[11], (unsigned int)s );
    glLogInfo( "-----------------------------\n" );

}

void glfwErrorCallback(int error, const char* description) {
    glLogErr("GLFW ERROR: code %i, message: %s\n", error, description);
}

void handleKeyEvents(GLFWwindow* window) {
    if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_ESCAPE)) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

void glfwWindowResizeCallback(GLFWwindow* window, int width, int height) {
    gWindowWidth = width;
    gWindowHeight = height;
}

void glfwFbufferResizeCallback(GLFWwindow* window, int width, int height) {
    gFbufferWidth = width;
    gFbufferHeight = height;
}

void _update_fps_counter( GLFWwindow* window ) {
    char tmp[128];

    static int frame_count;

    double current_seconds = glfwGetTime();
    double elapsed_seconds = current_seconds - previous_seconds;
    if (elapsed_seconds > 0.25) {
        previous_seconds = current_seconds;

        double fps = (double)frame_count / elapsed_seconds;
        sprintf(tmp, "opengl @ fps: %.2f", fps);
        glfwSetWindowTitle(window, tmp);
        frame_count = 0;
    }
    frame_count++;
}


int main(int argc, char const *argv[])
{
    GLFWwindow *window = nullptr;
    const GLubyte* renderer;
    const GLubyte* version;
    GLuint vao1, vbo1, vao2, vbo2;
    GLfloat shapePoints1[] = {
        0.5f, 0.5f, 0.0f,
        1.0f, -0.5f, 0.0f,
        0.0f, -0.5f, 0.0f,
    };
    GLfloat shapePoints2[] = {
        0.0f, 0.5f, -0.1f,
        0.5f, -0.5f, -0.1f,
        -0.5f, -0.5f, -0.1f,
    };

    /* these are the strings of code for the shaders
	the vertex shader positions each vertex point */
    const char *vertexShaderStr = "#version 410\n"
        "in vec3 pos;"
        "void main () {"
        "   gl_Position = vec4 (pos.x, pos.y, pos.z, 1.0);"
        "}";

    // Fragment Shader
    const char *fragmentShaderStr1 = "#version 410\n"
        "out vec4 colour;"
        "void main () {"
        "   colour = vec4(0.5, 0.0, 0.5, 1.0);"
        "}";
    
    // Fragment Shader
    const char *fragmentShaderStr2 = "#version 410\n"
        "out vec4 colour;"
        "void main () {"
        "   colour = vec4(0.5, 1.0, 0.5, 1.0);"
        "}";

    /* GL shader objects for vertex and fragment shader [components] */
	GLuint vertexShader, fragmentShader1, fragmentShader2;
	/* GL shader programme object [combined, to link] */
	GLuint shaderProgam1, shaderProgam2;

    // Initialize Log
    if (!restartGlLog()) { };

    // Start Context and O/S Window with GLFW
    glLogInfo("starting GLFW\n%s\n", glfwGetVersionString());

    // Set the glfw callbacks
    glfwSetErrorCallback(glfwErrorCallback);


    if (!glfwInit ()) {
        fprintf (stderr, "ERROR: could not start GLFW3\n");
        return 1;
    }
    fprintf(stdout, "INFO: started GLFW3!\n");
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 8);

    window = glfwCreateWindow(gWindowWidth, gWindowHeight, "Hello, Triangle!", nullptr, nullptr);
    if (!window) {
        fprintf(stderr, "ERROR: Failed to Initialize the Window");
        glfwTerminate();
        return 1;
    }
    // glfwSetWindowSizeCallback(window, glfwWindowResizeCallback);
    // glfwSetFramebufferSizeCallback(window, glfwFbufferResizeCallback);
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    glewInit();
    
    // Version Info
    renderer = glGetString(GL_RENDERER);
    version = glGetString(GL_VERSION);
    printf("Renderer: %s\n", renderer);
    glLogInfo("Renderer: %s\n", renderer);
    printf("OpenGL Version: %s\n", version);
    glLogInfo("OpenGL Version: %s\n", version);

    // Log GL Params
    logGLParms();

    // Tell GL only to draw into pixel is shape is closer to the viewer
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    /*
        Shape 1
    */
    // Generate the Vertex Buffer Object and Store the Data
    glGenBuffers(1, &vbo1);
    glBindBuffer(GL_ARRAY_BUFFER, vbo1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(shapePoints1), shapePoints1, GL_STATIC_DRAW);
    glGenVertexArrays(1, &vao1);
    glBindVertexArray(vao1);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    /*
        Shape 2
    */
    glGenBuffers(1, &vbo2);
    glBindBuffer(GL_ARRAY_BUFFER, vbo2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(shapePoints2), shapePoints2, GL_STATIC_DRAW);
    glGenVertexArrays(1, &vao2);
    glBindVertexArray(vao2);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    
    // Create and Compile Shaders
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderStr, nullptr);
    glCompileShader(vertexShader);

    fragmentShader1 = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader1, 1, &fragmentShaderStr1, nullptr);
    glCompileShader(fragmentShader1);

    shaderProgam1 = glCreateProgram();
    glAttachShader(shaderProgam1, fragmentShader1);
    glAttachShader(shaderProgam1, vertexShader);
    glLinkProgram(shaderProgam1);

    fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader2, 1, &fragmentShaderStr2, nullptr);
    glCompileShader(fragmentShader2);

    shaderProgam2 = glCreateProgram();
    glAttachShader(shaderProgam2, fragmentShader2);
    glAttachShader(shaderProgam2, vertexShader);
    glLinkProgram(shaderProgam2);


    
    while(!glfwWindowShouldClose(window)){
        // wipe the drawing buffer
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

        glUseProgram(shaderProgam1);
        glBindVertexArray(vao1);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glUseProgram(shaderProgam2);
        glBindVertexArray(vao2);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Update other events like input handling
        glfwPollEvents();
        handleKeyEvents(window);

        // Update Size of FB
        // glViewport(0, 0, gFbufferWidth, gFbufferHeight);

        // Put what we have drawn into the display
        glfwSwapBuffers(window);

        // Update FPS Display
        _update_fps_counter(window);
    }

    glfwTerminate();
    return 0;
}
