#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <iostream>
#include "Camera3D.h"
#include "../Scene.h"
#include <stdio.h>
#include <vector>


// In this file, inputs and creation of OpenGL context is managed
const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;
double gPreviousUpdateTime;
double gPreviousRenderTime;
GLFWwindow* gWindow;

float gTimeSinceLastSample;
std::vector<double> gDeltaTimes; // Would it be nice to calculate average deltaTime

bool initGL();

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInputKeys(GLFWwindow *window);
void processInputMouse(GLFWwindow* window, double xPos, double yPos);



int main()
{
    if (!initGL()) {
        std::cout << "Failed to initialize OpenGL" << std::endl;
    }

    glClearColor(0.f, 0.f, 0.f, 1.0f);  // Setup black as background
    initScene(SCREEN_WIDTH, SCREEN_HEIGHT);
    gPreviousUpdateTime = glfwGetTime();
    gPreviousRenderTime = glfwGetTime();

    // render loop
    // -----------
    while (!glfwWindowShouldClose(gWindow))
    {
        // input
        // -----
        processInputKeys(gWindow);

        glfwSetCursorPosCallback(gWindow, processInputMouse);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(gWindow);
        glfwPollEvents();

        // update (on MainThread.h)
        double updateDeltaTime = glfwGetTime() - gPreviousUpdateTime;
        gPreviousUpdateTime = glfwGetTime();
        updateGeometry(gPreviousUpdateTime, updateDeltaTime);
        // render (on MainThread.h)
        // ------
        double renderDeltaTime = glfwGetTime() - gPreviousRenderTime;
        gPreviousRenderTime = glfwGetTime();
        render(gPreviousRenderTime, renderDeltaTime);

        if (gTimeSinceLastSample > 1.0) {
            gTimeSinceLastSample = 0;
            gDeltaTimes.push_back(renderDeltaTime);
        }
        gTimeSinceLastSample += renderDeltaTime;
    }


    double acc = 0.0;
    for (int i = 0; i < gDeltaTimes.size(); i++) {
        acc += gDeltaTimes[i];
    }
    double avg = acc / gDeltaTimes.size();
    printf("Windows closed. Average fps was: %.1f", 1.0 / avg);
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInputKeys(GLFWwindow *window)
{
    // Exit
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    // Movement
    bool moveArray[6];
    moveArray[0] = glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS; // Up
    moveArray[1] = glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS; // Down
    moveArray[2] = glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS; // Left
    moveArray[3] = glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS; // Right
    moveArray[4] = glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS;
    moveArray[5] = glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS;
    gCamera->updatePosition(moveArray);

    // Rotation with keys
    float keyTurnSensitivity = 0.3f;
    bool turnArray[4];
    turnArray[0] = glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS;     // Left
    turnArray[1] = glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS;    // Right
    turnArray[2] = glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS;       // Up
    turnArray[3] = glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS;     // Down
    float turnX = (turnArray[1] - turnArray[0]) * keyTurnSensitivity;
    float turnY = (turnArray[3] - turnArray[2]) * keyTurnSensitivity;
    gCamera->turn(turnX, turnY);

}

void processInputMouse(GLFWwindow* window, double xPos, double yPos) {
    // Persistent last position
    static double lastX = xPos;
    static double lastY = yPos;
    static bool firstMouse = true;

    if (firstMouse) { // Avoid jump on first call
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    // Calculate mouse increment
    double deltaX = xPos - lastX;
    double deltaY = lastY - yPos; // For some glfw reason we have to invert y position

    lastX = xPos;
    lastY = yPos;

    // Send information to the camera
    gCamera->turn(static_cast<float>(deltaX), static_cast<float>(deltaY));
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
    gCamera->setScreenSize(width, height);

}