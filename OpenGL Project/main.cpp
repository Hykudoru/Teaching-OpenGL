#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Matrix.h"

int screenWidth = 800;
int screenHeight = 600;

// This code runs on the GPU
const char* vertexShaderSource = R"(
#version 330 core
layout(location = 0) in vec3 aPos;
void main() {
    gl_Position = vec4(aPos, 1.0);
}    
)";
// This code runs on the GPU
const char* fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;
void main() {
    FragColor = vec4(0.8, 0.3, 0.4, 1.0);
}    
)";

GLfloat vertices[] =
{
   -0.5f, 0.0f, 0.0f,
    0.0f, 0.8f, 0.0f,
    0.5f, 0.0f, 0.0f
};

GLuint shaderProgram;
static void SetupShaders()
{
    // Create a vertex shader object and specify it as a vertex shader type
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

    // Attach the vertex shader source code to the vertex shader object
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);

    // Compile the vertex shader to convert the source code into machine-readable instructions
    glCompileShader(vertexShader);

    // Create a fragment shader object and specify it as a fragment shader type
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    // Attach the fragment shader source code to the fragment shader object
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);

    // Compile the fragment shader to convert the source code into machine-readable instructions
    glCompileShader(fragmentShader);

    // Create a shader program to link the compiled shaders together
    shaderProgram = glCreateProgram();

    // Attach the compiled vertex shader to the shader program
    glAttachShader(shaderProgram, vertexShader);

    // Attach the compiled fragment shader to the shader program
    glAttachShader(shaderProgram, fragmentShader);

    // Link the shaders in the program, combining them into an executable for the GPU
    glLinkProgram(shaderProgram);

    // Delete the vertex shader object, as it is no longer needed after linking
    glDeleteShader(vertexShader);

    // Delete the fragment shader object, as it is no longer needed after linking
    glDeleteShader(fragmentShader);
}

GLuint VAO, VBO;
static void SetupBuffers()
{
    // Generating buffer will take a pointer to a GLuint and update it's value. 
    // We then can use that value later to specify which buffer we're using.
    // VAOs enable the GPU to retain attribute configurations, reducing the number of state changes and improving rendering performance.
    glGenVertexArrays(1, &VAO);// Must be generated before VBO. Very important.
    glBindVertexArray(VAO); // Bind it so we can use it.

    glGenBuffers(1, &VBO);// (how many buffers, pointer to an unsigned int)
    glBindBuffer(GL_ARRAY_BUFFER, VBO);// Bind it so we can use it.
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);// Must specify how big it is. (we can also use --> 9 * sizeof(float))

    /* Must specify the layout of our data using glVertexAttribPointer.
                                 GPU Buffer Memory Layout
                  |----------------------------------------------------|
                  |       Position         U,V           Normal        |
      Vertex  --->|   0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 2.0f, 2.0f, 2.0f   | |
                  |  |________________||__________||________________|  |
                  |      Attribute       Attribute      Attribute      |
                  |       12 bytes        8 bytes        12 bytes      |
                  |       index 0         index 1        index 2       |
                  |____________________________________________________|
    
     Param 1. Index is to which attribute you're referring to. The way our shader reads all this is by an index. Picture an array but types are different.
     Param 2. How many floats per vertex. 
     Param 3. Type
     Param 4. Normalized? If color = 255, must be normalized between 0 and 1. Can be done on CPU or have OpenGL do it for you.
     Param 5. Stride is the amount of bytes between each vertex. Above would be
     Param 6. Offset of the each attribute of one vertex.*/

    glVertexAttribPointer(0, 3, GL_FLOAT, false, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0); // Attribute index 

    glBindBuffer(GL_ARRAY_BUFFER, 0); // 0 = usually means no state
    glBindVertexArray(0); // 0 = usually means no state
}

// OpenGL operates like a state machine.
int main() 
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//Modern
    
    GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "OpenGL Project!", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "ERROR: Window couldn't open" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGL())
    {
        std::cout << "ERROR: Couldn't load glad." << std::endl;
        glfwTerminate();
        return -1;
    }

    SetupShaders();
    SetupBuffers();

    // bottom left to top right
    glViewport(0, 0, screenWidth, screenHeight);

    while (!glfwWindowShouldClose(window))
    {
        // Prepare to clear screen color and swap buffers to display the changed color.
        glClearColor(0.07f, 0.13f, 0.17f, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3); // (type, starting index, num to draw) example: index 1 = (0,0,0)
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Release the GPU resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
