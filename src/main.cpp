/***********************************************************************
** Main Loop. Manages Scene creation, Audio loading and stats text render
***********************************************************************/
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "ResourceManager.h"

#include <iostream>

#include <SDL.h>
#include <vector>

#include "AudioEngine.h"
#include "Scene.h"

#include "CubesTestingScene.h"
#include "FireScene.h"
// #include "LightTestScene.h"

#include "TextRenderer.h"

bool initGL();
// GLFW function declarations
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);

// We declare the window as a global variable
GLFWwindow* gWindow;

// The Width of the screen
const unsigned int SCREEN_WIDTH = 1920;
// The height of the screen
const unsigned int SCREEN_HEIGHT = 1080;

TextRenderer* textRenderer;
std::string statsText = "FPS: 0";

int main(int argc, char *argv[])
{
    if (!initGL()) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    std::cout << "Inited opengl context with width " << SCREEN_WIDTH <<", height: " << SCREEN_HEIGHT << std::endl;

    // Initialize audio and music
    if (!AudioEngine::Init()) {
        return -1;
    }
    // AudioEngine::LoadMusic("music", "assets/audio/electroswingLoop.mp3");
    // AudioEngine::SetMusicVolume(60);    // from 0 to 128
    // AudioEngine::PlayMusic("music", -1); // Play music on loop

    // Text renderer
    textRenderer = new TextRenderer(SCREEN_WIDTH, SCREEN_HEIGHT);
    textRenderer->Load("fonts/ocraext.ttf", 24);
    // initialize scene
    // ---------------
    // Scene::ChangeScene(new CubesTestingScene(SCREEN_WIDTH, SCREEN_HEIGHT));
    Scene::ChangeScene(new FireScene(SCREEN_WIDTH, SCREEN_HEIGHT));
    Scene::CurrentScene->Init();

    // deltaTime variables
    // -------------------
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    // Stats for performance
    float timeSinceLastStatUpdate = 0.0f;
    std::vector<float> deltaTimes;

    while (!glfwWindowShouldClose(gWindow))
    {
        // calculate delta time
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        // printf("dt: %.4f | FPS: %.1f\n", deltaTime, 1.0f / deltaTime);

        glfwPollEvents();

        // manage user input
        // -----------------
        Scene::CurrentScene->ProcessInput(deltaTime);

        // update scene state
        // -----------------
        Scene::CurrentScene->Update(deltaTime);

        // render
        // ------
        glClearColor(0.03f, 0.03f, 0.03f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Scene::CurrentScene->Render(deltaTime);


        // Update performance stats
        timeSinceLastStatUpdate += deltaTime;


        if (timeSinceLastStatUpdate > 1.0f) {
            timeSinceLastStatUpdate = 0.0f;
            deltaTimes.push_back(deltaTime);
            statsText = "FPS: " + std::to_string(static_cast<int>(1.0f / deltaTime));
        }
        glDisable(GL_DEPTH_TEST);
        textRenderer->RenderText(statsText, 20, 20, 1, glm::vec3(1, 1, 1));
        glEnable(GL_DEPTH_TEST);
        glfwSwapBuffers(gWindow);


    }

    // delete all resources as loaded using the resource manager
    // ---------------------------------------------------------
    ResourceManager::Clear();
    AudioEngine::Cleanup();
    // show performance stats
    // ----------------------
    double accDeltaTime = 0.0;
    for (int i = 0; i < deltaTimes.size(); i++) {
        accDeltaTime += deltaTimes[i];
    }
    double avg = accDeltaTime / deltaTimes.size();
    printf("Windows closed. Average fps was: %.1f", 1.0 / avg);

    glfwTerminate();
    return 0;
}

bool initGL() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    glfwWindowHint(GLFW_RESIZABLE, false);

    gWindow = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Breakout", nullptr, nullptr);
    glfwMakeContextCurrent(gWindow);
    glfwSetInputMode(gWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        return false;
    }

    // UNLEASH REAL POWER
    // Unlocks huge amount of fps (0)
    glfwSwapInterval(0);

    glfwSetKeyCallback(gWindow, key_callback);
    glfwSetCursorPosCallback(gWindow, mouse_callback); // <--- Esta es la del movimiento

    glfwSetFramebufferSizeCallback(gWindow, framebuffer_size_callback);

    // OpenGL configuration
    // --------------------
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    return true;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    // when a user presses the escape key, we set the WindowShouldClose property to true, closing the application
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
            Scene::CurrentScene->Keys[key] = true;
        else if (action == GLFW_RELEASE)
        {
            Scene::CurrentScene->Keys[key] = false;
            Scene::CurrentScene->KeysProcessed[key] = false;
        }
    }

}
float lastX = 400, lastY = 300;
bool firstMouse = true;
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    // Calculamos el desplazamiento
    Scene::CurrentScene->mouseDeltaX = xpos - lastX;
    Scene::CurrentScene->mouseDeltaY = lastY - ypos; // Invertido porque Y va de abajo a arriba en OpenGL

    lastX = xpos;
    lastY = ypos;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}