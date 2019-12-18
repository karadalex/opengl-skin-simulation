// Include C++ headers
#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Shader loading utilities and other
#include <common/shader.h>
#include <common/util.h>
#include <common/camera.h>
#include <common/model.h>
#include <common/texture.h>

using namespace std;
using namespace glm;

// Function prototypes
void initialize();
void createContext();
void mainLoop();
void free();

#define W_WIDTH 1024
#define W_HEIGHT 768
#define TITLE "SKIN PROJECT"

// Global variables
GLFWwindow* window;
Camera* camera;
GLuint shaderProgram;
GLuint projectionMatrixLocation, viewMatrixLocation, modelMatrixLocation;
GLuint lightLocation, timeLocation;
GLuint diffuceColorSampler, specularColorSampler;
GLuint diffuseTexture, specularTexture;

GLuint skinVAO;
GLuint skinVerticiesVBO, skinUVVBO, skinNormalsVBO;
std::vector<vec3> skinVertices, skinNormals;
std::vector<vec2> skinUVs;

GLuint sphereVAO;
GLuint sphereVerticiesVBO, sphereUVVBO, sphereNormalsVBO;
std::vector<vec3> sphereVertices, sphereNormals;
std::vector<vec2> sphereUVs;

GLuint objectVAOLocation, objectVAO;


void createContext()
{
    // Create and compile our GLSL program from the shaders
    shaderProgram = loadShaders(
        "skin-shader.vert",
        "skin-shader.frag"
    );

    // Homework 4: implement flat shading by transforming the normals of the model.

    // Task 6.2: load diffuse and specular texture maps
    diffuseTexture = loadSOIL("1K-human_skin_4_diffuseOriginal.jpg");
    // specularTexture = loadSOIL("suzanne_specular.bmp");

    // Task 6.3: get a pointer to the texture samplers (diffuseColorSampler, specularColorSampler)
    diffuceColorSampler = glGetUniformLocation(shaderProgram, "diffuceColorSampler");
    // specularColorSampler = glGetUniformLocation(shaderProgram, "specularColorSampler");

    // get pointers to the uniform variables
    projectionMatrixLocation = glGetUniformLocation(shaderProgram, "P");
    viewMatrixLocation = glGetUniformLocation(shaderProgram, "V");
    modelMatrixLocation = glGetUniformLocation(shaderProgram, "M");
    lightLocation = glGetUniformLocation(shaderProgram, "light_position_worldspace");
	timeLocation = glGetUniformLocation(shaderProgram, "time");
    objectVAOLocation = glGetUniformLocation(shaderProgram, "objectVAO");

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    
    // load skin
    loadOBJWithTiny("skin2.obj", skinVertices, skinUVs, skinNormals);

    // to attribute 1 and normals to attribute 2
    //*/
    glGenVertexArrays(1, &skinVAO);
    glBindVertexArray(skinVAO);

    // vertex VBO
    glGenBuffers(1, &skinVerticiesVBO);
    glBindBuffer(GL_ARRAY_BUFFER, skinVerticiesVBO);
    glBufferData(GL_ARRAY_BUFFER, skinVertices.size() * sizeof(glm::vec3),
        &skinVertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);

    // Homework 1: were the model normals not given, how would you compute them?
    glGenBuffers(1, &skinNormalsVBO);
    glBindBuffer(GL_ARRAY_BUFFER, skinNormalsVBO);
    glBufferData(GL_ARRAY_BUFFER, skinNormals.size() * sizeof(glm::vec3),
        &skinNormals[0], GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(1);

    // uvs VBO
    glGenBuffers(1, &skinUVVBO);
    glBindBuffer(GL_ARRAY_BUFFER, skinUVVBO);
    glBufferData(GL_ARRAY_BUFFER, skinUVs.size() * sizeof(glm::vec2),
        &skinUVs[0], GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(2);

    // load sphere
    loadOBJWithTiny("sphere.obj", sphereVertices, sphereUVs, sphereNormals);

    // to attribute 1 and normals to attribute 2
    //*/
    glGenVertexArrays(1, &sphereVAO);
    glBindVertexArray(sphereVAO);

    // vertex VBO
    glGenBuffers(1, &sphereVerticiesVBO);
    glBindBuffer(GL_ARRAY_BUFFER, sphereVerticiesVBO);
    glBufferData(GL_ARRAY_BUFFER, sphereVertices.size() * sizeof(glm::vec3),
        &sphereVertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);

    // Homework 1: were the model normals not given, how would you compute them?
    glGenBuffers(1, &sphereNormalsVBO);
    glBindBuffer(GL_ARRAY_BUFFER, sphereNormalsVBO);
    glBufferData(GL_ARRAY_BUFFER, sphereNormals.size() * sizeof(glm::vec3),
        &sphereNormals[0], GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(1);

    // uvs VBO
    glGenBuffers(1, &sphereUVVBO);
    glBindBuffer(GL_ARRAY_BUFFER, sphereUVVBO);
    glBufferData(GL_ARRAY_BUFFER, sphereUVs.size() * sizeof(glm::vec2),
        &sphereUVs[0], GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(2);

}

void free()
{
    // Delete skin
    glDeleteBuffers(1, &skinVerticiesVBO);
    glDeleteBuffers(1, &skinUVVBO);
    glDeleteBuffers(1, &skinNormalsVBO);
    glDeleteVertexArrays(1, &skinVAO);

    // Delete sphere
    glDeleteBuffers(1, &sphereVerticiesVBO);
    glDeleteBuffers(1, &sphereUVVBO);
    glDeleteBuffers(1, &sphereNormalsVBO);
    glDeleteVertexArrays(1, &sphereVAO);

    glDeleteTextures(1, &diffuseTexture);
    glDeleteProgram(shaderProgram);
    glfwTerminate();
}

void mainLoop()
{
    glm::vec3 lightPos = glm::vec3(0, 0, 4);

    do
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);

        // camera
        camera->update();

        glBindVertexArray(skinVAO);

        mat4 projectionMatrix = camera->projectionMatrix;
        mat4 viewMatrix = camera->viewMatrix;
        glm::mat4 modelMatrix = glm::mat4(1.0);

        float t = (float)glfwGetTime();

        // Task 1.4c: transfer uniforms to GPU
        glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
        glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
        glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);
        glUniform3f(lightLocation, lightPos.x, lightPos.y, lightPos.z); // light
		glUniform1f(timeLocation, t);

        glUniform1i(objectVAOLocation, skinVAO);

        // Task 6.4: bind textures and transmit diffuse and specular maps to the GPU
        //*/
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseTexture);
        glUniform1i(diffuceColorSampler, 1);

        // glActiveTexture(GL_TEXTURE1);
        // glBindTexture(GL_TEXTURE_2D, specularTexture);
        // glUniform1i(specularColorSampler, 1);
        //*/

        // draw
        glDrawArrays(GL_TRIANGLES, 0, skinVertices.size());

        glBindVertexArray(sphereVAO);
        glUniform1i(objectVAOLocation, sphereVAO);
        glDrawArrays(GL_TRIANGLES, 0, sphereVertices.size());

        glfwSwapBuffers(window);

        glfwPollEvents();
    } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
        glfwWindowShouldClose(window) == 0);
}

void initialize()
{
    // Initialize GLFW
    if (!glfwInit())
    {
        throw runtime_error("Failed to initialize GLFW\n");
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    window = glfwCreateWindow(W_WIDTH, W_HEIGHT, TITLE, NULL, NULL);
    if (window == NULL)
    {
        glfwTerminate();
        throw runtime_error(string(string("Failed to open GLFW window.") +
            " If you have an Intel GPU, they are not 3.3 compatible." +
            "Try the 2.1 version.\n"));
    }
    glfwMakeContextCurrent(window);

    // Start GLEW extension handler
    glewExperimental = GL_TRUE;

    // Initialize GLEW
    if (glewInit() != GLEW_OK)
    {
        glfwTerminate();
        throw runtime_error("Failed to initialize GLEW\n");
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    // Hide the mouse and enable unlimited movement
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Set the mouse at the center of the screen
    glfwPollEvents();
    glfwSetCursorPos(window, W_WIDTH / 2, W_HEIGHT / 2);

    // Gray background color
    glClearColor(0.5f, 0.5f, 0.5f, 0.0f);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    // enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // enable textures
    glEnable(GL_TEXTURE_2D);

    // Log
    logGLParameters();

    // Create camera
    camera = new Camera(window);
}

int main(void)
{
    try
    {
        initialize();
        createContext();
        mainLoop();
        free();
    }
    catch (exception& ex)
    {
        cout << ex.what() << endl;
        getchar();
        free();
        return -1;
    }

    return 0;
}
