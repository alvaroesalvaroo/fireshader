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
    mCamera = new Camera3D(2.f, 5.f, 45.0f);
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

    // Lot of unlited cubes
    Mesh* cubeWithEBO = new Mesh();
    cubeWithEBO->createCubeMeshWithNoEBO(0.75f);

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

    // If they go together, loads the texture to the previous open channel.
    Texture2D &text = ResourceManager::LoadTexture("floor", "textures/floor.jpg");
    ResourceManager::GetShader("TextureMatrix").SetTexture("textura", true, 0);

    err = glGetError();
    if (err != GL_NO_ERROR) {
        std::cerr << "Error loading textures: " <<  err <<std::endl;
    }
    // Apply texture to first and then replicate the same texture in the others
    int textureId = text.ID;
    for (int i = 0; i < NUM_CUBES; i++) {
        mUnlitCubes[i]->setTextureId(textureId);
    }

    // A single colored lit cube
    Mesh* cubeWithNormals= new Mesh();
    cubeWithNormals->createCubeWithNormals(0.75f);
    mLitCube->mMesh = cubeWithNormals;

    mLitCube->initShader("LitColorMatrix");
    // Texture2D &text = ResourceManager::LoadTexture("floor", "textures/floor.jpg"); // Main texture is selected, so there is no need to open new channel

    // mLitCube->loadTextureFromFile(mTextureFilename);
    mLitCube->setTextureId(textureId);
    mLitCube->setPosition(glm::vec3(-1.0f, 1.0f, 0.0f));

    // A cool textured lit cube
    Mesh* cubeWithNormalsAndUV = new Mesh();
    cubeWithNormalsAndUV->createCubeWithNormalsAndUV(2.f);
    mTexturedLitCube->mMesh = cubeWithNormalsAndUV;
    mTexturedLitCube->setTextureId(textureId);
    mTexturedLitCube->setPosition(glm::vec3(-0.0f, 0.0f, -3.0f));
    mTexturedLitCube->initShader("LitTexturedMatrix");

    // A light emissor
    Mesh* cube = new Mesh();
    cube->createCubeMeshWithNoEBO(0.15f);
    mLightEmissor->mMesh = cube;
    mLightEmissor->initLightShader();

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
    ResourceManager::GetShader("LitColorMatrix").SetVector3f("viewPosition", mCamera->getPosition());
    ResourceManager::GetShader("LitColorMatrix").SetVector3f("lightPosition", lightPosition);

    // Uniforms of LitTexturedMatrix textura, objectColor, lightPosition, viewPosition
    ResourceManager::GetShader("LitTexturedMatrix").SetTexture("textura", true, 0);
    ResourceManager::GetShader("LitTexturedMatrix").SetVector3f("viewPosition", mCamera->getPosition());
    ResourceManager::GetShader("LitTexturedMatrix").SetVector3f("lightColor", lightColor.x, lightColor.y, lightColor.z);
    ResourceManager::GetShader("LitTexturedMatrix").SetVector3f("lightPosition", lightPosition.x, lightPosition.y, lightPosition.z);

    // mTexturedLitCube->setIlumination(lightColor, lightPosition);


    mLightEmissor->setLightColor(lightColor);
    mLightEmissor->setPosition(lightPosition);

    bool hadError;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cerr << "Error pendiente al acabar Init: " << err << std::endl;
        hadError = true;
    }
    if (!hadError) {
        std::cout << "Initialization finished with no errors. Start rendering" << std::endl;
    }
}

bool firstError = true;
void CubesTestingScene::Render() {

    // GLenum err = glGetError();
    // err = glGetError();
    // if (err != GL_NO_ERROR && firstError) {
    //     firstError = false;
    //     std::cerr << "Error when rendering ct scene: " <<  err <<std::endl;
    // }
    double time = 0.0; // Placeholder porq asume que time/deltaTime son globales
    double deltaTime = 0.0; // Placeholder

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    // GameObject::render llama a activeTexture+bindTexture. Coloca las uniforms mvp y dibuja su mesh
    mLightEmissor->render(time, deltaTime, mCamera);

    for (int i = 0; i < NUM_CUBES; i++) {
        mUnlitCubes[i]->render(time, deltaTime, mCamera);
    }
    mLitCube->render(time, deltaTime, mCamera);
    ResourceManager::GetShader("LitTexturedMatrix").SetVector3f("viewPosition", mCamera->getPosition(), true);
    ResourceManager::GetShader("LitColorMatrix").SetVector3f("viewPosition", mCamera->getPosition(), true);
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

    mCamera->updatePosition(moveArray, dt);
}

float rotationSpeed = 0.0f;
float rotation = 0;
#define PI 3.14159265
void CubesTestingScene::Update(float dt) {
    mCamera->turn(mouseDeltaX, mouseDeltaY, dt);

    rotation += rotationSpeed * dt;
    if (rotation > 360) {
        rotation -= 360;
    }
    mTexturedLitCube->setRotation(glm::vec3(0.0f, 1.0f, 0.0f), rotation);

}