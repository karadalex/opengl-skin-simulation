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

#include "Skin.h"

namespace program2 {
    
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
GLuint diffuceColorSampler, ambientColorSampler;
GLuint diffuseTexture, ambientTexture;

Skin* skin;
GLuint objectVAOLocation, objectVAO;

// Skin transformations
GLuint skinTransformationsLocation;
vector<mat4> skinTransformations;

// Standard acceleration due to gravity
#define g 9.80665f

void loadVBO(GLuint VBO, std::vector<vec3> vertices);

void createContext() {
    // Create and compile our GLSL program from the shaders
    shaderProgram = loadShaders(
        "skin-shader.vert",
        "skin-shader.frag"
    );

    // load diffuse and ambient texture maps
    diffuseTexture = loadSOIL("textures/1K-human_skin_4_diffuseOriginal.jpg");
    ambientTexture = loadSOIL("textures/1K-human_skin_4_ao.jpg");

    // get a pointer to the texture samplers (diffuseColorSampler, ambientColorSampler)
    diffuceColorSampler = glGetUniformLocation(shaderProgram, "diffuceColorSampler");
    ambientColorSampler = glGetUniformLocation(shaderProgram, "ambientColorSampler");

    // get pointers to the uniform variables
    projectionMatrixLocation = glGetUniformLocation(shaderProgram, "P");
    viewMatrixLocation = glGetUniformLocation(shaderProgram, "V");
    modelMatrixLocation = glGetUniformLocation(shaderProgram, "M");
    lightLocation = glGetUniformLocation(shaderProgram, "light_position_worldspace");
	timeLocation = glGetUniformLocation(shaderProgram, "time");
    objectVAOLocation = glGetUniformLocation(shaderProgram, "objectVAO");
    skinTransformationsLocation = glGetUniformLocation(shaderProgram, "skinTransformations");

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    skin = new Skin("models/skin3.obj");
}


void free() {
    delete skin;

    glDeleteTextures(1, &diffuseTexture);
    glDeleteProgram(shaderProgram);
    glfwTerminate();
}


void mainLoop() {
    glm::vec3 lightPos = glm::vec3(0, 0, 4);
    float t = glfwGetTime();

    do {
        // calculate dt
        float currentTime = glfwGetTime();
        // float dt = currentTime - t;
		float dt = 0.01f;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);

        camera->update();

		glUniform1i(objectVAOLocation, skin->skin->VAO);
        // Exert forces in multiple particles
		for (auto particle : skin->particles) {
			// If vertex is in the boundary of the mesh, it should not move
            if (particle->isInBoundary) {
                vector<float> f(6, 0.0f);
                particle->addForce(f);
            }
            else {
				float forceX = 2 + particle->k * (particle->a.x - particle->x.x) - 2*particle->b * particle->v.x;
				float forceY = -particle->m * g + particle->k*(particle->a.y - particle->x.y) - particle->b*particle->v.y;
                vector<float> f(6, 0.0f);
                f[0] = forceX;
                f[1] = forceY;
                particle->addForce(f);
            }
		}

        // Exert force in one particle, approximately in the center
        auto particle = skin->particles.at((int)skin->numOfParticles/2);
		cout << particle->v.y << ' ' << particle->x.y << endl;

		skin->update(t, dt);
        skin->draw();

        mat4 projectionMatrix = camera->projectionMatrix;
        mat4 viewMatrix = camera->viewMatrix;
        glm::mat4 modelMatrix = glm::mat4(1.0);

        float t = (float)glfwGetTime();

        // transfer uniforms to GPU
        glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
        glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
        glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);
        glUniform3f(lightLocation, lightPos.x, lightPos.y, lightPos.z); // light
		glUniform1f(timeLocation, t);

        // load skin transformations
		skinTransformations.clear();
        for (auto particle : skin->particles)
        {
            skinTransformations.push_back(translate(mat4(1.0), particle->x));
        }
        glUniformMatrix4fv(skinTransformationsLocation, skinTransformations.size(), GL_FALSE, &skinTransformations[0][0][0]);

		// bind textures and transmit diffuse and ambient maps to the GPU
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseTexture);
        glUniform1i(diffuceColorSampler, 1);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, ambientTexture);
        glUniform1i(ambientColorSampler, 1);

        glfwSwapBuffers(window);

        // Update time by dt
        t += dt;

        glfwPollEvents();
    } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

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

}