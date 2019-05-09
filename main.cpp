#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
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

using namespace std;
using namespace glm;

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
        cerr << "OpenGL debug. Message: " << endl <<
            string(message, length) << endl;
    }
}
#pragma GCC diagnostic pop

int main()
{
    srand(time(nullptr));

    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_SAMPLES, 4);


    /* Create a windowed mode window and its OpenGL context */
    int screenWidth = 1900, screenHeight = 1210;
    GLFWwindow *window = glfwCreateWindow(screenWidth, screenHeight, "Screensaver", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        cerr << "Error!" << endl;
    }

    const char *ver = (const char *) glGetString(GL_VERSION);
    cout << ver << endl;

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);

    // Set the debug callback function
    // TODO: DISABLE THIS FOR RELEASE
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(debugCallback, nullptr);

    double prevTime;
    double curTime = prevTime = glfwGetTime();
    double deltaTime = 0.0;

    // Init meshes
    vector<Vertex> vertices;

    // Loading mesh from file
    fstream fs;
    fs.open("meshes/triangle.txt");
    vertices = {};
    for (int i = 0; i < 3; ++i) {
        float vx, vy, vz, tx, ty;
        fs >> vx >> vy >> vz >> tx >> ty;
        vertices.push_back({vec3(vx, vy, vz), vec2(tx, ty)});
    }
    Mesh meshTriangleSet(vertices);

    vertices = {
        {{-1.0, -sqrt(3.0)/3.0, 0.0 }, {}},
        {{ 1.0, -sqrt(3.0)/3.0, 0.0 }, {}},
        {{ 0.0, 2.0 * sqrt(3.0)/3.0, 0.0 }, {}}, 
    };
    Mesh meshTriangle(vertices);

    vertices = {
        {{ -1.0, -1.0, 0.0 }, {0.0, 0.0 }}, 
        {{ -1.0, 1.0, 0.0 }, {0.0, 1.0}}, 
        {{ 1.0, 1.0, 0.0 }, {1.0, 1.0}}, 
        {{ 1.0, 1.0, 0.0 }, {1.0, 1.0}}, 
        {{ 1.0, -1.0, 0.0 }, {1.0, 0.0 }}, 
        {{ -1.0, -1.0, 0.0 }, {0.0, 0.0 }}
    };
    Mesh meshSquare(vertices);

    vertices = {
        {{ 0.0, -1.0, 2.0 }, {}}, 
        {{ -sqrt(3), -1.0, -1.0 }, {}}, 
        {{ sqrt(3), -1.0, -1.0 }, {}},

        {{ 0.0, 2.0, 0.0 }, {}}, 
        {{ -sqrt(3), -1.0, -1.0 }, {}}, 
        {{ sqrt(3), -1.0, -1.0 }, {}},

        {{ 0.0, 2.0, 0.0 }, {}}, 
        {{ 0.0, -1.0, 2.0 }, {}}, 
        {{ sqrt(3), -1.0, -1.0 }, {}},

        {{ 0.0, 2.0, 0.0 }, {}}, 
        {{ 0.0, -1.0, 2.0 }, {}}, 
        {{ -sqrt(3), -1.0, -1.0 }, {}},
    };
    Mesh meshTetra(vertices);

    vertices = {
        {{ -1.0, -1.0, -1.0 },  {0.0, 0.0 }},
        {{1.0, -1.0, -1.0 },  {1.0, 0.0 }},
        {{1.0,  1.0, -1.0 },  {1.0, 1.0 }},
        {{1.0,  1.0, -1.0 },  {1.0, 1.0 }},
        {{-1.0,  1.0, -1.0 },  {0.0, 1.0 }},
        {{-1.0, -1.0, -1.0 },  {0.0, 0.0 }},

        {{-1.0, -1.0,  1.0 },  {0.0, 0.0 }},
        {{1.0, -1.0,  1.0 },  {1.0, 0.0 }},
        {{1.0,  1.0,  1.0 },  {1.0, 1.0 }},
        {{1.0,  1.0,  1.0 },  {1.0, 1.0 }},
        {{-1.0,  1.0,  1.0 },  {0.0, 1.0 }},
        {{-1.0, -1.0,  1.0 },  {0.0, 0.0 }},

        {{-1.0,  1.0,  1.0 },  {1.0, 0.0 }},
        {{-1.0,  1.0, -1.0 },  {1.0, 1.0 }},
        {{-1.0, -1.0, -1.0 },  {0.0, 1.0 }},
        {{-1.0, -1.0, -1.0 },  {0.0, 1.0 }},
        {{-1.0, -1.0,  1.0 },  {0.0, 0.0 }},
        {{-1.0,  1.0,  1.0 },  {1.0, 0.0 }},

        {{1.0,  1.0,  1.0 },  {1.0, 0.0 }},
        {{1.0,  1.0, -1.0 },  {1.0, 1.0 }},
        {{1.0, -1.0, -1.0 },  {0.0, 1.0 }},
        {{1.0, -1.0, -1.0 },  {0.0, 1.0 }},
        {{1.0, -1.0,  1.0 },  {0.0, 0.0 }},
        {{1.0,  1.0,  1.0 },  {1.0, 0.0 }},

        {{-1.0, -1.0, -1.0 },  {0.0, 1.0 }},
        {{1.0, -1.0, -1.0 },  {1.0, 1.0 }},
        {{1.0, -1.0,  1.0 },  {1.0, 0.0 }},
        {{1.0, -1.0,  1.0 },  {1.0, 0.0 }},
        {{-1.0, -1.0,  1.0 },  {0.0, 0.0 }},
        {{-1.0, -1.0, -1.0 },  {0.0, 1.0 }},

        {{-1.0,  1.0, -1.0 },  {0.0, 1.0 }},
        {{1.0,  1.0, -1.0 },  {1.0, 1.0 }},
        {{1.0,  1.0,  1.0 },  {1.0, 0.0 }},
        {{1.0,  1.0,  1.0 },  {1.0, 0.0 }},
        {{-1.0,  1.0,  1.0 },  {0.0, 0.0 }},
        {{-1.0,  1.0, -1.0 },  {0.0, 1.0}}
    };
    Mesh meshCube(vertices);

    Shader shaderBasic("shaders/basic.vert", "shaders/basic.frag");

    int TRCNT = 7;
    vector<vec3> trSetPos(TRCNT);
    vector<vec3> trSetRotAxis(TRCNT);
    vector<vec3> trSetCenter(TRCNT);
    vector<float> trSetRotV(TRCNT);
    vector<float> trSetRotAngle(TRCNT);
    vector<vec4> trSetColor(TRCNT);
    for (int i = 0; i < TRCNT; ++i) {
        float cx = (float)rand()/RAND_MAX * 5 - 8;
        float cy = (float)rand()/RAND_MAX * 8 - 4;
        float cz = (float)rand()/RAND_MAX * 8 - 4;
        float ax = (float)rand()/RAND_MAX * 2 - 1;
        float ay = (float)rand()/RAND_MAX * 2 - 1;
        float az = (float)rand()/RAND_MAX * 2 - 1;
        float colorx = (float)rand()/RAND_MAX * 0.5 + 0.5;
        float colory = (float)rand()/RAND_MAX * 0.5 + 0.5;
        float colorz = (float)rand()/RAND_MAX * 0.5 + 0.5;
        //float colorw = (float)rand()/RAND_MAX * 2 - 1;
        trSetCenter[i] = vec3(cx, cy, cz);
        trSetRotAxis[i] = vec3(ax, ay, az);
        trSetRotV[i] = (float)rand()/RAND_MAX * 10 - 5;
        trSetRotAngle[i] = 0.0;
        trSetColor[i] = vec4(colorx, colory, colorz, 0.6);
    }

    
    vec3 trPos = {0.0, 5.0, 0.0};
    vec3 trRotAxis = normalize(vec3{1.0, 0.3, 1.5});
    float trRotAngle = 0.0; // in radians

    vec3 sqPos = {0.0, 0.0, 0.0};
    vec3 sqRotAxis = normalize(vec3{3.3, 1.2, 0.1});
    float sqRotAngle = 0.0; // in radians

    vec3 cubePos = {7.0, 1.0, 2.0};
    vec3 cubeRotAxis = normalize(vec3{1.0, 1.0, -1.0});
    float cubeRotAngle = 0.0; // in radians

    vec3 tetraPos = { 1.0, -6.0, -6.0 };
    vec3 tetraRotAxis = normalize(vec3{ 1.0, 1.0, 1.0 });
    float tetraRotAngle = 0.0; // in radians

    Shader shaderTextured("shaders/textured.vert", "shaders/textured.frag");
    shaderTextured.SetUniform("tex", 0);
    Texture textureMetal("textures/metal.jpg");
    Texture textureCmc("textures/cmc.png");

    Shader shaderBg("shaders/background.vert", "shaders/textured.frag");
    shaderBg.SetUniform("tex", 0);
    Texture textureBg("textures/bg.jpg");

    glClearColor(0.0, 0.0, 0.0, 1.0);
    

    // main loop
    while (glfwWindowShouldClose(window) == GL_FALSE) {
        // Update
        glfwPollEvents();
        prevTime = curTime;
        curTime = glfwGetTime();
        deltaTime = curTime - prevTime;

        trPos.x = sin(curTime);
        trRotAngle += deltaTime * 2.0;
        
        for (int i = 0; i < TRCNT; ++i) {
            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
                trSetCenter[i].y = glm::min(5.0, trSetCenter[i].y + deltaTime);
            }
            if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
                trSetCenter[i].y = glm::max(-5.0, trSetCenter[i].y - deltaTime);
            }
            trSetPos[i].x = trSetCenter[i].x + cos(curTime);
            trSetPos[i].y = trSetCenter[i].y + sin(curTime);
            trSetRotAngle[i] += deltaTime * trSetRotV[i];
        }

        sqPos.y = sin(curTime);
        sqRotAngle += deltaTime * 2.0;

        cubePos.x = 5.0 + cos(curTime);
        cubePos.y = 0.0 + sin(curTime);
        cubeRotAngle += deltaTime * 4.0;

        tetraPos.x = 3.0 + sin(curTime) * 2;
        tetraRotAngle += deltaTime * 3.0;

        // Render
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // P * V * M
        // Projection
        mat4 PV = perspective((float)radians(45.0),
                (float)(screenWidth) / screenHeight,
                0.1f, 100.0f);
        // View
        PV = translate(PV, vec3(0.0, 0.0, -15.0));
        // Model
        
        // background
        shaderBg.Use();
        textureBg.Bind();
        meshSquare.Draw();
    

        // triangle
        shaderBasic.SetUniform("myColor", vec4(0.3, 1.0, 0.1, 0.5));
        mat4 transMat = translate(PV, trPos);
        transMat = rotate(transMat, trRotAngle, trRotAxis);
        shaderBasic.SetUniform("transformMat", transMat);
        shaderBasic.Use();
        meshTriangle.Draw();

        // trSet
        for (int i = 0; i < TRCNT; ++i) {
            shaderBasic.SetUniform("myColor", trSetColor[i]);
            mat4 transMat = translate(PV, trSetPos[i]);
            transMat = rotate(transMat, trSetRotAngle[i], trSetRotAxis[i]);
            shaderBasic.SetUniform("transformMat", transMat);
            shaderBasic.Use();
            meshTriangle.Draw();
        }

        // square
        transMat = translate(PV, sqPos);
        transMat = rotate(transMat, sqRotAngle, sqRotAxis);
        shaderTextured.SetUniform("transformMat", transMat);
        shaderTextured.Use();
        textureMetal.Bind();
        meshSquare.Draw();
        
        // cube
        transMat = translate(PV, cubePos);
        transMat = rotate(transMat, cubeRotAngle, cubeRotAxis);
        shaderTextured.SetUniform("transformMat", transMat);
        shaderTextured.Use();
        textureCmc.Bind();
        meshCube.Draw();

        // tetra
        transMat = translate(PV, tetraPos);
        transMat = rotate(transMat, tetraRotAngle, tetraRotAxis);
        shaderBasic.SetUniform("transformMat", transMat);
        shaderBasic.SetUniform("myColor", vec4(0.8, 0.8, 0.5, 0.5));
        shaderBasic.Use();
        meshTetra.Draw();


        glfwSwapBuffers(window);


    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
