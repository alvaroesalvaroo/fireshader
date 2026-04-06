//
// Created by alv18 on 29/04/2025.
//

#include "CubesTestingScene.h"
// Basic
#include <stdio.h>
// Own modules
#include "Rendering/Shader.h"
// OpenGl
#include <glad/glad.h>

// Texture loader
#include "stb_image.h"

// Maths
#include <cmath>
#include "glm.hpp"
#include "../3DOldProject/Objects/TexturedUnlitCube.h"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "../3DOldProject/Objects/LightEmissorCube.h"
#include "../3DOldProject/Objects/LitCube.h"
#include "../3DOldProject/Objects/TexturedLitPlane.h"
#include "GLFW/glfw3.h"

using namespace glm;

CubesTestingScene::CubesTestingScene(int width, int height)
    : screenWidth(width), screenHeight(height) {
    mLitCube = new LitCube();
    mTexturedLitCube = new TexturedLitPlane();
    mLightEmissor = new LightEmissorCube();
    mCamera = new Camera3D(0.05f, 0.001f, 45.0f);
}

CubesTestingScene::~CubesTestingScene() {
    for (int i = 0; i < NUM_CUBES; i++) {
        delete mCubes[i];
    }
    delete mLitCube;
    delete mTexturedLitCube;
    delete mLightEmissor;
}

void CubesTestingScene::Init() {
    glClearColor(0, 0.0, 0.2, 1.0);

    // Camera
    mCamera->setScreenSize(screenWidth, screenHeight);
    mCamera->setPosition(0.0f, 0.0f, 5.0f);

    // Lot of unlited cubes
    for (int i = 0; i < NUM_CUBES; i++) {
        mCubes[i] = new TexturedUnlitCube();
        mCubes[i]->generateCube(0.75f);
        mCubes[i]->initKnownShader();
        mCubes[i]->setPosition(glm::vec3((float)i, -1.0f, 0.0f));
    }
    // Apply texture to first and then replicate the same texture in the others
    int textureId = mCubes[0]->loadTextureFromFile(mTextureFilename, 1);
    for (int i = 1; i < NUM_CUBES; i++) {
        mCubes[i]->setTextureId(textureId);
    }

    // A single colored lit cube
    mLitCube->generateCube(1.0);
    mLitCube->initKnownShader();
    // mLitCube->loadTextureFromFile(mTextureFilename);
    mLitCube->setTextureId(textureId);
    mLitCube->setPosition(glm::vec3(-1.0f, 1.0f, 0.0f));

    // A cool textured lit cube
    mTexturedLitCube->generateCube(1.0);
    mTexturedLitCube->initKnownShader();
    mTexturedLitCube->setTextureId(textureId);
    mTexturedLitCube->setPosition(glm::vec3(-1.0f, 0.0f, -3.0f));

    // A light emissor
    mLightEmissor->generateCube(1.0f);
    mLightEmissor->initKnownShader();

    // Decide light position and color
    glm::vec3 objectColor = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 lightColor = glm::vec3(0.5f, 1.0f, 1.0f);
    glm::vec3 lightPosition = glm::vec3(1.0f, -0.0f, -1.0f);

    // Configure lighting settigs for lit objects
    mLitCube->setObjectColor(objectColor);
    mLitCube->setIlumination(lightColor, lightPosition);

    mTexturedLitCube->setIlumination(lightColor, lightPosition);

    mLightEmissor->setLightColor(lightColor);
    mLightEmissor->setPosition(lightPosition);

    printf("Initialization finished. Start rendering\n");
}

void CubesTestingScene::Render() {
    // Nota: 'time' y 'deltaTime' deberían venir de tu motor/loop principal.
    // Aquí se asume que tienes acceso a ellos o los pasas por miembros de clase.
    double time = 0.0; // Placeholder
    double deltaTime = 0.0; // Placeholder

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    for (int i = 0; i < NUM_CUBES; i++) {
        mCubes[i]->render(time, deltaTime, mCamera);
    }

    mLightEmissor->render(time, deltaTime, mCamera);
    mLitCube->render(time, deltaTime, mCamera);
    mTexturedLitCube->render(time, deltaTime, mCamera);
}

void CubesTestingScene::ProcessInput(float dt) {
    bool moveArray[6] = { false };

    if (this->Keys[GLFW_KEY_W]) moveArray[0] = true; // Forward
    if (this->Keys[GLFW_KEY_S]) moveArray[1] = true; // Backward
    if (this->Keys[GLFW_KEY_A]) moveArray[2] = true; // Left
    if (this->Keys[GLFW_KEY_D]) moveArray[3] = true; // Right
    if (this->Keys[GLFW_KEY_E]) moveArray[4] = true; // Up
    if (this->Keys[GLFW_KEY_Q]) moveArray[5] = true; // Down

    mCamera->updatePosition(moveArray);
}

void CubesTestingScene::Update(float dt) {

}