/*******************************************************************
** This code is part of Scene::CurrentScene->
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "GameLogic/Game.h"
#include "ResourceManager.h"

#include <iostream>

#include "ResourceManager.h"
#include <SDL.h>

#include "AudioEngine.h"
#include "GameLogic/Selector.h"
#include "GameLogic/TwoPlayersGame.h"

#define TWO_PLAYERS_GAME

bool initGL();
// GLFW function declarations
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// We declare the window as a global variable
GLFWwindow* gWindow;

// The Width of the screen
const unsigned int SCREEN_WIDTH = 1200;
// The height of the screen
const unsigned int SCREEN_HEIGHT = 800;

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
    AudioEngine::LoadMusic("music", "assets/audio/electroswingLoop.mp3");
    AudioEngine::SetMusicVolume(60);    // from 0 to 128
    AudioEngine::PlayMusic("music", -1); // Play music on loop

    // initialize game
    // ---------------
    Scene::ChangeScene(new Selector(SCREEN_WIDTH, SCREEN_HEIGHT));

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
        glfwPollEvents();

        // manage user input
        // -----------------
        Scene::CurrentScene->ProcessInput(deltaTime);

        // update game state
        // -----------------
        Scene::CurrentScene->Update(deltaTime);

        // render
        // ------
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        Scene::CurrentScene->Render();

        glfwSwapBuffers(gWindow);

        // Update performance stats
        timeSinceLastStatUpdate += deltaTime;
        if (timeSinceLastStatUpdate > 1.0f) {
            timeSinceLastStatUpdate = 0.0f;
            deltaTimes.push_back(deltaTime);
        }
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

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}