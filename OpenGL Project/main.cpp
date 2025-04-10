#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>
#include <vector>
#include "Matrix.h"
#include "stb_image.h"
const float PI = 3.14159f;

float ToRad(float deg) {
    return deg * PI / 180.0;
}
int screenWidth = 1600;
int screenHeight = 900;
float fov = ToRad(60);
float aspect = (float)screenHeight / (float)screenWidth;

GLuint LoadTexture(const char* path) {
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // horizontal wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // vertical wrapping

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // minified filter
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // magnified filter

    int width, height, nrChannels;
    unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
    if (data) {
        GLenum format = (nrChannels == 3) ? GL_RGB : GL_RGBA;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to load texture." << std::endl;
    }

    stbi_image_free(data);
    return textureID;
}

class Transform
{
public:
    static std::vector<Transform*> objects;
    float scale;
    Vec3 position;
    Matrix3x3 rotation;
    
    Transform(float size, Vec3 pos, Matrix3x3 rot) : scale(size), position(pos), rotation(rot)
    {
        objects.emplace_back(this);
    }
};
std::vector<Transform*> Transform::objects = std::vector<Transform*>();

double deltaTime = 0;

void Time()
{
    static double prevTime = 0;
    double currentTime = glfwGetTime();
    deltaTime = currentTime - prevTime;
    prevTime = currentTime;
}

// This code runs on the GPU
const char* vertexShaderSource = R"(
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aUV;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

out vec3 color;
out vec2 uv;

void main() {
    vec4 pos = u_model * vec4(aPos, 1.0);

    float w = pos.z;
    pos.x /= pos.z;
    pos.y /= pos.z;
    pos.z /= -10000.0;
    gl_Position = pos;
    color = aColor;
    uv = aUV;
}    
)";

// This code runs on the GPU
//const char* fragmentShaderSource = R"(
//#version 330 core
//
//in vec3 color;
//
//out vec4 FragColor;
//
//void main() {
//    FragColor = vec4(color, 1.0);
//}    
//)";

// This code runs on the GPU
const char* fragmentShaderSource = R"(
#version 330 core

in vec3 color;
in vec2 uv;
uniform sampler2D u_texture;

out vec4 FragColor;

void main() {
    FragColor = vec4(color, 1.0); //texture(u_texture, uv);// * vec4(color, 1.0);
}    
)";

GLfloat vertices[] =
{
    //      Position                  RGB                  UV
       // Front Face
        0.5f, -0.5f, -0.5f,    0.0f, 0.0f, 1.0f,      1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,      0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f,    0.0f, 1.0f, 0.0f,      0.0f, 1.0f,
        0.5f, 0.5f, -0.5f,     0.0f, 0.0f, 1.0f,      1.0f, 1.0f,

        // Back Face
        0.5f, -0.5f, 0.5f,     0.0f, 0.0f, 1.0f,      1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f,    1.0f, 0.0f, 0.0f,      0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f,     0.0f, 1.0f, 0.0f,      0.0f, 1.0f,
        0.5f, 0.5f, 0.5f,      0.0f, 0.0f, 1.0f,      1.0f, 1.0f,

        // Right face
        0.5f, -0.5f, 0.5f,     0.0f, 0.0f, 1.0f,      1.0f, 0.0f,
        0.5f, -0.5f, -0.5f,    1.0f, 0.0f, 0.0f,      0.0f, 0.0f,
        0.5f, 0.5f, -0.5f,     0.0f, 1.0f, 0.0f,      0.0f, 1.0f,
        0.5f, 0.5f, 0.5f,      0.0f, 0.0f, 1.0f,      1.0f, 1.0f,

        // Left face
        -0.5f, -0.5f, 0.5f,    0.0f, 0.0f, 1.0f,      1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,      0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f,    0.0f, 1.0f, 0.0f,      0.0f, 1.0f,
        -0.5f, 0.5f, 0.5f,     0.0f, 0.0f, 1.0f,      1.0f, 1.0f,

        // Top face
        0.5f, 0.5f, -0.5f,     0.0f, 0.0f, 1.0f,      1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f,    1.0f, 0.0f, 0.0f,      0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f,     0.0f, 1.0f, 0.0f,      0.0f, 1.0f,
        0.5f, 0.5f, 0.5f,      0.0f, 0.0f, 1.0f,      1.0f, 1.0f,

        // Bottom face
        0.5f, -0.5f, -0.5f,    0.0f, 0.0f, 1.0f,      1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,      0.0f, 0.0f,
        -0.5f, -0.5f, 0.5f,    0.0f, 1.0f, 0.0f,      0.0f, 1.0f,
        0.5f, -0.5f, 0.5f,     0.0f, 0.0f, 1.0f,      1.0f, 1.0f,
};

GLuint indices[] = {
    // Front side triangles
    0, 1, 2,
    0, 2, 3,

    // Back side triangles
    5, 4, 7,
    5, 7, 6,

    // Right side triangles
    8, 9, 10,
    8, 10, 11,

    // Left side triangles
    13, 12, 15,
    13, 15, 14,

    // Top side triangles
    16, 17, 18,
    16, 18, 19,

    // Bottom side triangles
    23, 22, 21,
    23, 21, 20
};

Vec3 position;
Matrix3x3 rotation = Matrix3x3::identity;

static void DebugCompiledShader(GLuint shader)
{
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER COMPILATION FAILED\n" << infoLog << std::endl;
    }
}

static GLuint SetupShaders()
{
    // Create a vertex shader object and specify it as a vertex shader type
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

    // Attach the vertex shader source code to the vertex shader object
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);

    // Compile the vertex shader to convert the source code into machine-readable instructions
    glCompileShader(vertexShader);

    // Shader Compilation Debugging
    DebugCompiledShader(vertexShader);

    // Create a fragment shader object and specify it as a fragment shader type
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    // Attach the fragment shader source code to the fragment shader object
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);

    // Compile the fragment shader to convert the source code into machine-readable instructions
    glCompileShader(fragmentShader);

    // Shader Compilation Debugging
    DebugCompiledShader(fragmentShader);

    // Create a shader program to link the compiled shaders together
    GLuint shaderProgram = glCreateProgram();

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

    return shaderProgram;
}

GLuint VAO, VBO, EBO;
static void SetupBuffers()
{
    // Generating buffer will take a pointer to a GLuint and update it's value. 
    // We then can use that value later to specify which buffer we're using.
    // VAOs enable the GPU to retain attribute configurations, reducing the number of state changes 
    // and improving rendering performance.
    glGenVertexArrays(1, &VAO);// Must be generated before VBO. Very important.
    glBindVertexArray(VAO); // Bind it so we can use it.

    glGenBuffers(1, &VBO);// (how many buffers, pointer to an unsigned int)
    glBindBuffer(GL_ARRAY_BUFFER, VBO);// Bind it so we can use it.
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);// Must specify how big it is. (we can also use --> 9 * sizeof(float))

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);// Bind it so we can use it.
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);// Must specify how big it is. (we can also use --> 9 * sizeof(float))

    /* Must specify the layout of our data using glVertexAttribPointer.
                                 GPU Buffer Memory Layout
                  |----------------------------------------------------|
                  |       Position         U,V           Normal        |
      Vertex  --->|   0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 2.0f, 2.0f, 2.0f   |
                  |  |________________||__________||________________|  |
                  |      Attribute       Attribute      Attribute      |
                  |       12 bytes        8 bytes        12 bytes      |
                  |       index 0         index 1        index 2       |
                  |____________________________________________________|

                            GPU Buffer Memory Layout
                  |-----------------------------------------|
                  |       Position             RGB          |
      Vertex  --->|   0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f    |
                  |  |________________||________________|   |
                  |      Attribute         Attribute        |
                  |       12 bytes          12 bytes        |
                  |       index 0           index 1         |
                  |_________________________________________|
    
     Param 1. Index is to which attribute you're referring to. The way our shader reads all this is by an index. Picture an array but types are different.
     Param 2. How many floats per vertex. 
     Param 3. Type
     Param 4. Normalized? If color = 255, must be normalized between 0 and 1. Can be done on CPU or have OpenGL do it for you.
     Param 5. Stride is the amount of bytes between each vertex. Above would be
     Param 6. Offset of the each attribute of one vertex.*/

    // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, false, 8 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0); // Attribute index 
    // RGB
    glVertexAttribPointer(1, 3, GL_FLOAT, false, 8 * sizeof(GLfloat), (void*)(3*sizeof(GLfloat)));
    glEnableVertexAttribArray(1); // Attribute index 
    // UV Coords
    glVertexAttribPointer(2, 2, GL_FLOAT, false, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2); // Attribute index 

    glBindBuffer(GL_ARRAY_BUFFER, 0); // 0 = usually means no state
    glBindVertexArray(0); // 0 = usually means no state
}

Transform* ship1 = nullptr;

void Init()
{
    ship1 = new Transform(1, Vec3(0, 0, 0), Matrix3x3::identity);
    new Transform(1, Vec3(0, 0, 0), Matrix3x3::identity);
}

void Update() 
{
    
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
        std::cout << "ERROR: Failed to create window." << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGL())
    {
        std::cout << "ERROR: Failed to load glad." << std::endl;
        glfwTerminate();
        return -1;
    }

    GLuint shaderProgram = SetupShaders();
    SetupBuffers();
    GLuint uniModelID = glGetUniformLocation(shaderProgram, "u_model");
    //GLuint textureID = LoadTexture("tile_floor.jpg");
    //glUniform1i(glGetUniformLocation(shaderProgram, "u_texture"), 0);

    // bottom left to top right
    glViewport(0, 0, screenWidth, screenHeight);

    glEnable(GL_DEPTH_TEST);

    Init();
    Update();

    while (!glfwWindowShouldClose(window))
    {
        Time();
        // Prepare to clear screen color and swap buffers to display the changed color.
        glClearColor(0.07f, 0.13f, 0.17f, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glUseProgram(shaderProgram);

        static int frame = 0;
        frame += 1;

        for (size_t i = 0; i < Transform::objects.size(); i++)
        {
            Transform* transform = Transform::objects[i];
            transform->scale = 1;//0.1f + abs(cos(frame * 0.0005f));
            transform->position = Vec3(i, 0.0f, 10.0f * -abs(cos(frame * PI * 0.005f)));
            transform->rotation = Matrix3x3::RotY((PI / 4.0f) * deltaTime) * transform->rotation;// *Matrix3x3::RotX((3.14159f / 4.0f) * deltaTime);
            float modelMatrixTRS[4][4] = {
                {transform->scale * transform->rotation.m[0][0], transform->scale * transform->rotation.m[0][1], transform->scale * transform->rotation.m[0][2], transform->position.x},
                {transform->scale * transform->rotation.m[1][0], transform->scale * transform->rotation.m[1][1], transform->scale * transform->rotation.m[1][2], transform->position.y},
                {transform->scale * transform->rotation.m[2][0], transform->scale * transform->rotation.m[2][1], transform->scale * transform->rotation.m[2][2], transform->position.z},
                {0.0f, 0.0f, 0.0f, 1.0f},
            };
            
            glUniformMatrix4fv(uniModelID, 1, GL_TRUE, (const float*)modelMatrixTRS);//Must declare after shader program
            
            //glActiveTexture(GL_TEXTURE0);
            //glBindTexture(GL_TEXTURE_2D, textureID);

            glBindVertexArray(VAO);
            //glDrawArrays(GL_TRIANGLES, 0, 36);
            // Draw the object using indices.
            // This approach reduces memory usage by eliminating duplicate vertices and allows OpenGL to reuse vertices for drawing each face of the cube.
            glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(GLuint), GL_UNSIGNED_INT, 0);
        }
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Release the GPU resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}



//using namespace std;

//void DebugGPU()
//{
//    GLint maxTextures;
//    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextures);
//    cout << "Max Supported Texture Units: " << maxTextures << endl;
//
//    const char* vendor = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
//    const char* renderer = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
//
//    cout << "Vendor: " << vendor << endl;
//    cout << "Renderer: " << renderer << endl;
//
//    if (std::string(vendor).find("NVIDIA") != std::string::npos) {
//        cout << "NVIDIA GPU" << endl;
//    }
//    else if (std::string(vendor).find("AMD") != std::string::npos || std::string(vendor).find("ATI") != std::string::npos) {
//        cout << "AMD GPU" << endl;
//    }
//}


//// This code runs on the GPU
//const char* defaultVertexShaderSource = R"(
//#version 330 core
//layout(location = 0) in vec3 aPos;
//layout(location = 1) in vec3 aColor;
//
//uniform mat4 u_model;
//
//out vec3 color;
//
//void main() {
//    gl_Position = u_model * vec4(aPos, 1.0);
//    color = aColor;
//}    
//)";
//
//// This code runs on the GPU
//const char* defaultFragmentShaderSource = R"(
//#version 330 core
//
//in vec3 color;
//
//out vec4 FragColor;
//
//void main() {
//    FragColor = vec4(color, 1.0);
//}    
//)";
//
//// This code runs on the GPU
//const char* textureVertexShaderSource = R"(
//#version 330 core
//layout(location = 0) in vec3 aPos;
//layout(location = 1) in vec3 aColor;
//layout(location = 2) in vec2 aUV;
//
//uniform mat4 u_model;
//
//out vec3 color;
//out vec2 uv;
//
//void main() {
//    gl_Position = u_model * vec4(aPos, 1.0);
//    color = aColor;
//    uv = aUV;
//}    
//)";
//
//// This code runs on the GPU
//const char* textureFragmentShaderSource = R"(
//#version 330 core
//
//in vec3 color;
//in vec2 uv;
//
//uniform sampler2D u_texture;
//
//out vec4 FragColor;
//
//void main() {
//    FragColor = texture(u_texture, uv);// * vec4(color, 0);
//}    
//)";

//static GLuint SetupShaders(const char* vertexShaderSource, const char* fragmentShaderSource)
//{
//    // Create a vertex shader object and specify it as a vertex shader type
//    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
//
//    // Attach the vertex shader source code to the vertex shader object
//    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
//
//    // Compile the vertex shader to convert the source code into machine-readable instructions
//    glCompileShader(vertexShader);
//
//    // Shader Compilation Debugging
//    DebugCompiledShader(vertexShader);
//
//    // Create a fragment shader object and specify it as a fragment shader type
//    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
//
//    // Attach the fragment shader source code to the fragment shader object
//    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
//
//    // Compile the fragment shader to convert the source code into machine-readable instructions
//    glCompileShader(fragmentShader);
//
//    // Shader Compilation Debugging
//    DebugCompiledShader(fragmentShader);
//
//    // Create a shader program to link the compiled shaders together
//    GLuint shaderProgram = glCreateProgram();
//
//    // Attach the compiled vertex shader to the shader program
//    glAttachShader(shaderProgram, vertexShader);
//
//    // Attach the compiled fragment shader to the shader program
//    glAttachShader(shaderProgram, fragmentShader);
//
//    // Link the shaders in the program, combining them into an executable for the GPU
//    glLinkProgram(shaderProgram);
//
//    // Delete the vertex shader object, as it is no longer needed after linking
//    glDeleteShader(vertexShader);
//
//    // Delete the fragment shader object, as it is no longer needed after linking
//    glDeleteShader(fragmentShader);
//
//    return shaderProgram;
//}

//// OpenGL operates like a state machine.
//int main()
//{
//    glfwInit();
//
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//Modern
//
//    GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "OpenGL Project!", NULL, NULL);
//    if (window == NULL)
//    {
//        std::cout << "ERROR: Failed to create window." << std::endl;
//        glfwTerminate();
//        return -1;
//    }
//    glfwMakeContextCurrent(window);
//
//    if (!gladLoadGL())
//    {
//        std::cout << "ERROR: Failed to load glad." << std::endl;
//        glfwTerminate();
//        return -1;
//    }
//
//    // bottom left to top right
//    glViewport(0, 0, screenWidth, screenHeight);
//
//    glEnable(GL_DEPTH_TEST);
//
//    DebugGPU();
//
//    GLuint defaultShaderProgram = SetupShaders(defaultVertexShaderSource, defaultFragmentShaderSource);
//    GLuint textureShaderProgram = SetupShaders(textureVertexShaderSource, textureFragmentShaderSource);
//
//    SetupBuffers();
//
//    glUniform1i(glGetUniformLocation(textureShaderProgram, "u_texture"), 0);
//    GLuint textures[] = {
//        LoadTexture("tile_floor.jpg"),
//        LoadTexture("wood_floor.jpg")
//    };
//
//    while (!glfwWindowShouldClose(window))
//    {
//        // Prepare to clear screen color and swap buffers to display the changed color.
//        glClearColor(0.07f, 0.13f, 0.17f, 1.0);
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//        Time();
//
//        static int frame = 0;
//        frame += 1;
//
//        rotation = rotation * Matrix3x3::RotY((3.14159f / 4.0f) * deltaTime) * Matrix3x3::RotX((3.14159f / 4.0f) * deltaTime);
//        for (size_t i = 0; i < 2; i++)
//        {
//            float scale = 0.1f + abs(cos(frame * 0.0005f));
//            position = Vec3(i, 0.0f, 0.0f);
//            float model[4][4] = {
//                {scale * rotation.m[0][0], scale * rotation.m[0][1], scale * rotation.m[0][2], 0.0f},
//                {scale * rotation.m[1][0], scale * rotation.m[1][1], scale * rotation.m[1][2], 0.0f},
//                {scale * rotation.m[2][0], scale * rotation.m[2][1], scale * rotation.m[2][2], 0.0f},
//                {position.x, position.y, position.z, 1.0f},
//            };
//
//            if (i == 0) {
//                glUseProgram(textureShaderProgram);
//                glUniformMatrix4fv(glGetUniformLocation(textureShaderProgram, "u_model"), 1, GL_FALSE, (const float*)model);//Must declare after shader program
//            }
//            else {
//                glUseProgram(defaultShaderProgram);
//                glUniformMatrix4fv(glGetUniformLocation(defaultShaderProgram, "u_model"), 1, GL_FALSE, (const float*)model);//Must declare after shader program
//            }
//
//            glActiveTexture(GL_TEXTURE0);
//            glBindTexture(GL_TEXTURE_2D, textures[i]);
//
//            glBindVertexArray(VAO);
//            //glDrawArrays(GL_TRIANGLES, 0, 36);
//            // Draw the object using indices.
//            // This approach reduces memory usage by eliminating duplicate vertices and allows OpenGL to reuse vertices for drawing each face of the cube.
//            glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(GLuint), GL_UNSIGNED_INT, 0);
//        }
//        glfwSwapBuffers(window);
//        glfwPollEvents();
//    }
//
//    // Release the GPU resources
//    glDeleteVertexArrays(1, &VAO);
//    glDeleteBuffers(1, &VBO);
//    glDeleteProgram(defaultShaderProgram);
//
//    glfwDestroyWindow(window);
//    glfwTerminate();
//    return 0;
//}