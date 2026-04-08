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
#include <iostream>
#include <ostream>

#include "glm.hpp"
#include "ResourceManager.h"
#include "gtc/type_ptr.hpp"
#include "LightEmissor.h"

#include "GLFW/glfw3.h"

using namespace glm;

CubesTestingScene::CubesTestingScene(int width, int height)
    : screenWidth(width), screenHeight(height) {
    mLitCube = new Object3D();
    mTexturedLitCube = new Object3D();
    mLightEmissor = new LightEmissor();
    mCamera = new Camera3D(0.05f, 0.001f, 45.0f);
}

CubesTestingScene::~CubesTestingScene() {
    for (int i = 0; i < NUM_CUBES; i++) {
        delete mUnlitCubes[i];
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

    Mesh* cubeWithEBO = new Mesh();
    cubeWithEBO->createCubeMeshWithEBO(0.75f);
    // Lot of unlited cubes
    for (int i = 0; i < NUM_CUBES; i++) {
        mUnlitCubes[i] = new Object3D();
        mUnlitCubes[i]->mMesh = cubeWithEBO;
        // mUnlitCubes[i]->generateCube(0.75f);
        mUnlitCubes[i]->initShader("TextureMatrix");
        mUnlitCubes[i]->setPosition(glm::vec3((float)i, -1.0f, 0.0f));
    }
    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        std::cerr << "Error initing Textured Unlit Cubes: " <<  err <<std::endl;
    }
    // Apply texture to first and then replicate the same texture in the others
    int textureId = mUnlitCubes[0]->loadTextureFromFile(mTextureFilename, 1);
    for (int i = 1; i < NUM_CUBES; i++) {
        mUnlitCubes[i]->setTextureId(textureId);
    }

    // A single colored lit cube
    Mesh* cubeWithNormals= new Mesh();
    cubeWithNormals->createCubeWithNormals(0.75f);
    mLitCube->mMesh = cubeWithNormals;

    mLitCube->initShader("LitColorMatrix");
    // mLitCube->loadTextureFromFile(mTextureFilename);
    mLitCube->setTextureId(textureId);
    mLitCube->setPosition(glm::vec3(-1.0f, 1.0f, 0.0f));

    // A cool textured lit cube

    mTexturedLitCube->mMesh = cubeWithNormals;
    mTexturedLitCube->setTextureId(textureId);
    mTexturedLitCube->setPosition(glm::vec3(-1.0f, 0.0f, -3.0f));
    mTexturedLitCube->initShader("LitTexturedMatrix");

    // A light emissor
    Mesh* cube = new Mesh();
    cube->createCubeMeshWithNoEBO(0.75f);
    mLightEmissor->mMesh = cube;
    mLightEmissor->initKnownShader();

    // Decide light position and color
    glm::vec3 objectColor = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 lightColor = glm::vec3(0.5f, 1.0f, 1.0f);
    glm::vec3 lightPosition = glm::vec3(1.0f, -0.0f, -1.0f);

    // Configure lighting settigs for lit objects
    err = glGetError();
    if (err != GL_NO_ERROR) {
        std::cerr << "Error before setting up lighting: " <<  err <<std::endl;
    }
    // Uniforms of LitColorMatrix: lightColor, objectColor, lightPosition, viewPosition
    ResourceManager::GetShader("LitColorMatrix").SetVector3f("lightColor", lightColor.x, lightColor.y, lightColor.z, true);
    ResourceManager::GetShader("LitColorMatrix").SetVector3f("objectColor", objectColor.x, objectColor.y, objectColor.z);

    // Uniforms of LitTexturedMatrix textura, objectColor, lightPosition, viewPosition
    ResourceManager::GetShader("LitTexturedMatrix").SetVector3f("lightColor", lightColor.x, lightColor.y, lightColor.z, true);
    ResourceManager::GetShader("LitTexturedMatrix").SetVector3f("lightPosition", lightPosition.x, lightPosition.y, lightPosition.z, true);

    // mTexturedLitCube->setIlumination(lightColor, lightPosition);


    mLightEmissor->setLightColor(lightColor);
    mLightEmissor->setPosition(lightPosition);

    err = glGetError();
    if (err != GL_NO_ERROR) {
        std::cerr << "Error when ending scene: " <<  err <<std::endl;
    }
    printf("Initialization finished. Start rendering\n");
}

void CubesTestingScene::Render() {

    // GLenum err = glGetError();
    // if (err != GL_NO_ERROR) {
    //     std::cout << "Error when rendering ct scene: " <<  err <<std::endl;
    // }
    double time = 0.0; // Placeholder porq asume que time/deltaTime son globales
    double deltaTime = 0.0; // Placeholder

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    for (int i = 0; i < NUM_CUBES; i++) {
        mUnlitCubes[i]->render(time, deltaTime, mCamera);
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