#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

#include "Shader.h"
#include "Texture.h"
#include "Mesh.h"
#include "Vertex.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>
#include <cstdlib>
#include <ctime>

#pragma GCC diagnostic ignored "-Wunused-parameter"
void debugCallback(GLenum source,
        GLenum type,
        GLuint id,
        GLenum severity,
        GLsizei length,
        const GLchar *message,
        const void *userParam)
{
    if (severity != GL_DEBUG_SEVERITY_NOTIFICATION) {
        std::cerr << "OpenGL debug. Message: " << std::endl <<
            std::string(message, length) << std::endl;
    }
}
#pragma GCC diagnostic pop

extern std::pair<std::vector<Vertex>, std::vector<GLushort>> ReadMesh(const std::string &path);


int main()
{
    srand(time(nullptr));

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    /* Create a windowed mode window and its OpenGL context */
    int screenWidth = 1200;
    int screenHeight = 720;
    GLFWwindow *window = glfwCreateWindow(screenWidth, screenHeight, "Computer Graphics", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Init GLEW */
    if (glewInit() != GLEW_OK) {
        std::cerr << "Error!" << std::endl;
    }

    const char *ver = (const char *) glGetString(GL_VERSION);
    std::cout << ver << std::endl;

    // Set the debug callback function
    // TODO: DISABLE THIS FOR RELEASE
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(debugCallback, nullptr);

    glEnable(GL_DEPTH_TEST);

    double prevTime;
    double curTime = prevTime = glfwGetTime();
    double deltaTime = 0.0;

    // Init meshes
    std::vector<Vertex> vertices;
    std::vector<GLushort> emptyInds = {};
    // triangle
    // vertex + texture map(0..1)
    vertices = {
        {{-1.0, -sqrt(3.0)/3.0, 0.0}, {0.0, 0.0}},
        {{ 1.0, -sqrt(3.0)/3.0, 0.0}, {1.0, 0.0}},
        {{ 0.0, 2.0 * sqrt(3.0)/3.0, 0.0}, {0.5, sqrt(3.0)/4.0}}, 
    };
    Mesh meshTriangle(vertices, emptyInds);

    vertices = {
        {{ -1.0, -1.0, 0.0 }, {}}, 
        {{ -1.0, 1.0, 0.0 }, {}}, 
        {{ 1.0, 1.0, 0.0 }, {}}, 
        {{ 1.0, 1.0, 0.0 }, {}}, 
        {{ 1.0, -1.0, 0.0 }, {}}, 
        {{ -1.0, -1.0, 0.0 }, {}}
    };
    Mesh meshSquare(vertices, emptyInds);

    vertices = {
        {{-0.5f, -0.5f, -0.5f},  {0.0f, 0.0f}},
        {{0.5f, -0.5f, -0.5f},  {1.0f, 0.0f}},
        {{0.5f,  0.5f, -0.5f},  {1.0f, 1.0f}},
        {{0.5f,  0.5f, -0.5f},  {1.0f, 1.0f}},
        {{-0.5f,  0.5f, -0.5f},  {0.0f, 1.0f}},
        {{-0.5f, -0.5f, -0.5f},  {0.0f, 0.0f}},

        {{-0.5f, -0.5f,  0.5f},  {0.0f, 0.0f}},
        {{0.5f, -0.5f,  0.5f},  {1.0f, 0.0f}},
        {{0.5f,  0.5f,  0.5f},  {1.0f, 1.0f}},
        {{0.5f,  0.5f,  0.5f},  {1.0f, 1.0f}},
        {{-0.5f,  0.5f,  0.5f},  {0.0f, 1.0f}},
        {{-0.5f, -0.5f,  0.5f},  {0.0f, 0.0f}},

        {{-0.5f,  0.5f,  0.5f},  {1.0f, 0.0f}},
        {{-0.5f,  0.5f, -0.5f},  {1.0f, 1.0f}},
        {{-0.5f, -0.5f, -0.5f},  {0.0f, 1.0f}},
        {{-0.5f, -0.5f, -0.5f},  {0.0f, 1.0f}},
        {{-0.5f, -0.5f,  0.5f},  {0.0f, 0.0f}},
        {{-0.5f,  0.5f,  0.5f},  {1.0f, 0.0f}},

        {{0.5f,  0.5f,  0.5f},  {1.0f, 0.0f}},
        {{0.5f,  0.5f, -0.5f},  {1.0f, 1.0f}},
        {{0.5f, -0.5f, -0.5f},  {0.0f, 1.0f}},
        {{0.5f, -0.5f, -0.5f},  {0.0f, 1.0f}},
        {{0.5f, -0.5f,  0.5f},  {0.0f, 0.0f}},
        {{0.5f,  0.5f,  0.5f},  {1.0f, 0.0f}},

        {{-0.5f, -0.5f, -0.5f},  {0.0f, 1.0f}},
        {{0.5f, -0.5f, -0.5f},  {1.0f, 1.0f}},
        {{0.5f, -0.5f,  0.5f},  {1.0f, 0.0f}},
        {{0.5f, -0.5f,  0.5f},  {1.0f, 0.0f}},
        {{-0.5f, -0.5f,  0.5f},  {0.0f, 0.0f}},
        {{-0.5f, -0.5f, -0.5f},  {0.0f, 1.0f}},

        {{-0.5f,  0.5f, -0.5f},  {0.0f, 1.0f}},
        {{0.5f,  0.5f, -0.5f},  {1.0f, 1.0f}},
        {{0.5f,  0.5f,  0.5f},  {1.0f, 0.0f}},
        {{0.5f,  0.5f,  0.5f},  {1.0f, 0.0f}},
        {{-0.5f,  0.5f,  0.5f},  {0.0f, 0.0f}},
        {{-0.5f,  0.5f, -0.5f},  {0.0f, 1.0}}
    };
    Mesh meshCube(vertices, emptyInds);

    Shader shaderBasic("shaders/basic.vert", "shaders/basic.frag");
    //shaderBasic.SetUniform("myColor", glm::vec4(0.3, 1.0, 0.1, 1.0));
    
    glm::vec3 trPos = {0.0, 0.0, 0.0};
    glm::vec3 trRotAxis = glm::normalize(glm::vec3{1.0, 0.3, 1.5});
    float trRotAngle = 0.0; // in radians

    glm::vec3 sqPos = {-3.5, 0.0, 0.0};
    glm::vec3 sqRotAxis = glm::normalize(glm::vec3{3.3, 1.2, 0.1});
    float sqRotAngle = 0.0; // in radians

    glm::vec3 cubePos = {7.0, 1.0, 0.0};
    glm::vec3 cubeRotAxis = glm::normalize(glm::vec3{1.0, 1.0, -1.0});
    float cubeRotAngle = 0.0; // in radians

    glClearColor(0.0, 0.0, 0.0, 1.0);

    // main loop
    while (glfwWindowShouldClose(window) == GL_FALSE) {
        // Update
        glfwPollEvents();
        prevTime = curTime;
        curTime = glfwGetTime();
        deltaTime = curTime - prevTime;

        trPos.x = std::sin(curTime);
        trRotAngle += deltaTime * 2.0;

        sqPos.y = std::sin(curTime);
        sqRotAngle += deltaTime * 2.0;

        cubePos.x = 5.0 + std::cos(curTime);
        cubePos.y = 0.0 + std::sin(curTime);
        cubeRotAngle += deltaTime * 4.0;

        // Render
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // fullTransform = P * V * M
        // P = Projection
        glm::mat4 PV = glm::perspective(static_cast<float>(glm::radians(45.0)),
                static_cast<float>(screenWidth) / screenHeight,
                0.1f, 100.0f);
        // V = View
        PV = glm::translate(PV, glm::vec3(0.0, 0.0, -15.0));
        // M = Model
        
    

        // triangle
        shaderBasic.SetUniform("myColor", glm::vec4(0.3, 1.0, 0.1, 1.0));
        glm::mat4 transMat = glm::translate(PV, trPos);
        transMat = glm::rotate(transMat, trRotAngle, trRotAxis);
        shaderBasic.SetUniform("transformMat", transMat);

        shaderBasic.Use();
        meshTriangle.Draw();

        // square
        shaderBasic.SetUniform("myColor", glm::vec4(1.0, 0.0, 0.0, 1.0));
        transMat = glm::translate(PV, sqPos);
        transMat = glm::rotate(transMat, sqRotAngle, sqRotAxis);
        shaderBasic.SetUniform("transformMat", transMat);

        shaderBasic.Use();
        meshSquare.Draw();
        
        // cube
        shaderBasic.SetUniform("myColor", glm::vec4(0.0, 0.0, 1.0, 1.0));
        transMat = glm::translate(PV, cubePos);
        transMat = glm::rotate(transMat, cubeRotAngle, cubeRotAxis);
        shaderBasic.SetUniform("transformMat", transMat);

        shaderBasic.Use();
        meshCube.Draw();


        glfwSwapBuffers(window);


    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
