//
// Created by alv18 on 29/04/2025.
//

// Basic
#include <iostream>
// OpenGl
#include "glad/glad.h"

#include "GLFW/glfw3.h"

// Maths
#include "glm.hpp"
#include "gtc/type_ptr.hpp"

// Own modules
#include "FireScene.h"
#include "Shader.h"
#include "LightEmissor.h"
#include "Material.h"
#include "ResourceManager.h"
#include "Spark.h"

using namespace glm;




FireScene::FireScene(int width, int height) : ::Scene(width, height) {
    // mLitCube = new Object3D();
    // mTexturedLitCube = new Object3D();
    mLightPositionsUniforms.clear();
    mLights.clear();
    mCamera = new Camera3D(2.f, 5.f, 45.0f);
    mGround = new Object3D();
}

FireScene::~FireScene() {
}

#define SPARKS_NUMBER 15

void FireScene::Init() {

    // Handly debug
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // ====== GROUND ====== //
    Mesh* groundMesh = new Mesh();
    groundMesh->loadMeshFromFile(GroundMeshFilename);
    mGround->setMesh(groundMesh);

    Shader& litShader = ResourceManager::LoadShader(GroundShaderName);
    mGround->setShader(&litShader);

    // Ground texture
    Texture2D& groundTex = ResourceManager::LoadTexture("ground", GroundTextureFilename);
    litShader.SetTexture("textura", true, 0);
    mGround->setTextureId(groundTex.ID);

    // Normal map texture
    Texture2D& normalTex = ResourceManager::LoadTexture("normal", GroundNormalTextureFilename);
    litShader.SetTexture("normalMap", true, 1);
    mGround->setSecondaryTextureId(normalTex.ID); // Commenting this gives funny results

    // Ground material
    Material* mat = new Material(0.1f, 0.8f, 0.2f);
    mat->updateMaterialToShader(&ResourceManager::GetShader(GroundShaderName));

    mGround->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    // Link lit shader with camera and lights

    litShader.SetVector3f("viewPosition", mCamera->getPosition());

    // ===== SPARKS ======= //
    Mesh* cubeMesh = new Mesh();
    cubeMesh->createCubeMeshWithNoEBO(0.01);
    for (int i = 0; i < SPARKS_NUMBER; i++) {
        Spark* spark = new Spark();
        mLights.push_back(spark);
        mLights[i]->setMesh(cubeMesh);
    }
    Shader &emissionShader = ResourceManager::LoadShader("LightEmissor");

    // Setup lights
    glm::vec3 lightColor = glm::vec3(1, 0.7, 0);
    for (int i = 0; i < mLights.size(); i++) {
        // Shader emissive
        mLights[i]->setShader(&emissionShader);
        mLights[i]->setLight(lightColor, 1, 1.0, 0.1, 2);  // Solo asigna mPointLight, sin GL todavía

        // Shader receptor
        mLights[i]->initLightUniformIntoShader(&litShader, i, true);
        // Cachear uniforms de light position, se usan mucho en el render
        std::string uniformName = "lights[" + std::to_string(i) + "].position";
        GLint positionUniform = glGetUniformLocation(litShader.getID(), uniformName.c_str());
        mLightPositionsUniforms.push_back(positionUniform);
    }
    // Importante: light count en el receptor
    litShader.SetInteger("lightCount", mLights.size(), true);

    // AUX LIGHT
    // LightEmissor* lightEmissor = new LightEmissor();
    // LightEmissor* lightEmissor2 = new LightEmissor();
    // mLights.push_back(lightEmissor);
    // mLights.push_back(lightEmissor2);

    // ==== SMOKE ===== //
    mSmoke = new Object3D();
    mSmoke->setPosition(0.f, 0.25f, 0.f);
    mSmoke->setScale(1.f, 1.5f, 1.f);
    Mesh* plane = new Mesh();
    plane->generatePlane(1);
    mSmoke->setMesh(plane);
    Shader& smokeShader = ResourceManager::LoadShader(SmokeShaderName);
    mSmoke->setShader(&smokeShader);
    Texture2D& smokeTex = ResourceManager::LoadTexture("smoke", SmokeTextureName);
    smokeShader.SetTexture("billboardTex", true, 0);
    mSmoke->setTextureId(smokeTex.ID);

    Texture2D& noiseTex = ResourceManager::LoadTexture("noise", NoiseTextureName);
    smokeShader.SetTexture("noiseTex", true, 1);
    mSmoke->setSecondaryTextureId(noiseTex.ID);
    // glBindTexture(GL_TEXTURE_2D, noiseTex.ID);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // glBindTexture(GL_TEXTURE_2D, 0);
    smokeShader.SetFloat("time", 0); // Init time uniform

    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        std::cerr << "GLError initing scene: " << err << std::endl;
        return;
    }
    std::cout << "Init Fire Scene with no errors " << std::endl;

}

void FireScene::ProcessInput(float dt) {
    bool moveArray[6] = { false };

    if (this->Keys[GLFW_KEY_W]) moveArray[0] = true; // Forward
    if (this->Keys[GLFW_KEY_S]) moveArray[1] = true; // Backward
    if (this->Keys[GLFW_KEY_A]) moveArray[2] = true; // Left
    if (this->Keys[GLFW_KEY_D]) moveArray[3] = true; // Right
    if (this->Keys[GLFW_KEY_E]) moveArray[4] = true; // Up
    if (this->Keys[GLFW_KEY_Q]) moveArray[5] = true; // Down

    mCamera->updatePosition(moveArray, dt);
}

float totalTime = 0;

void FireScene::Render(float dt) {
    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // glEnable(GL_DEPTH_TEST);

    for (auto light : mLights) {
        light->render(dt, mCamera);
    }
    // gSmokeAttemp->render(dt, mCamera);

    Shader& litShader = ResourceManager::GetShader(GroundShaderName);

    litShader.SetVector3f("viewPosition", mCamera->getPosition(), true);

    for (int i = 0; i < mLights.size(); i++) {
        // PUede ser optimizado cacheando la uniform:
        // mLights[i]->updateLightPositionIntoShader(&litShader, i, false);

        // std::string uniformName = "lights[" + std::to_string(i) + "].position";
        // litShader.SetVector3f(uniformName.c_str(), mLights[i]->getPosition());

        // Optimización (practicamente imperceptible)
        glm::vec3 lightPos = mLights[i]->getPosition();
        glUniform3f(mLightPositionsUniforms[i], lightPos.x, lightPos.y, lightPos.z);

    }

    mGround->render(dt, mCamera);

    // Billboard
    glDepthMask(GL_FALSE);        // no escribe en depth buffer
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    ResourceManager::GetShader(SmokeShaderName).SetFloat("time", totalTime, true);
    mSmoke->render(dt, mCamera);
    glDepthMask(GL_TRUE);         // restaurar
}

#define PI 3.14159265


void FireScene::Update(float dt) {
    totalTime += dt;

    for (int i = 0; i < mLights.size(); i++) {
        mLights[i]->update(dt); // Since theyre sparks, update method is override
    }

    mCamera->turn(mouseDeltaX, mouseDeltaY, dt);
}



