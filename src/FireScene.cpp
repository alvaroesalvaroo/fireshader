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
    mFakeSparks.clear();
    mCamera = new Camera3D(2.f, 5.f, 45.0f);
    mGround = new Object3D();
}

FireScene::~FireScene() {
}

#define SPARKS_NUMBER 15
#define FAKE_SPARKS_NUMBER 15

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

    // ===== REAL SPARKS ======= //

    // Mesh* cubeMesh = new Mesh();
    // cubeMesh->createCubeMeshWithNoEBO(0.01);
    Mesh* sparkMesh = new Mesh();
    sparkMesh->loadMeshFromFile(SparkMeshFilename);
    Mesh* sparkMesh2 = new Mesh();
    sparkMesh2->loadMeshFromFile(SparkMeshFilename2);

    for (int i = 0; i < SPARKS_NUMBER; i++) {
        Spark* spark = new Spark();
        mLights.push_back(spark);
        mLights[i]->setMesh(sparkMesh);
    }
    Shader &emissionShader = ResourceManager::LoadShader("LightEmissor");


    // ==== SMOKE ===== //
    mSmoke = new Object3D();
    mSmoke->setPosition(0.f, 0.75f, 0.f);
    // mSmoke->setScale(2.f, 3.f, 2.f);
    Mesh* plane = new Mesh();
    plane->generatePlane(1);
    Mesh* trapezium = new Mesh();
    trapezium->generateTrapezium(2, 1, 3.f);
    mSmoke->setMesh(trapezium);

    Shader& smokeShader = ResourceManager::LoadShader(SmokeShaderName);
    mSmoke->setShader(&smokeShader);

    Texture2D& smokeTex = ResourceManager::LoadTexture("smoke", SmokeTextureName);
    smokeShader.SetTexture("billboardTex", true, 0);
    mSmoke->setTextureId(smokeTex.ID);

    Texture2D& noiseTex = ResourceManager::LoadTexture("noise", NoiseTextureName);
    smokeShader.SetTexture("noiseTex", true, 1);
    mSmoke->setSecondaryTextureId(noiseTex.ID);

    smokeShader.SetFloat("time", 0); // Init time uniform

    // ====== FLAME =========//
    mFlame = new Object3D();
    mFlame->setMesh(trapezium);
    mFlame->setPosition(glm::vec3(0.0f, 0.5f, 0.1f));
    mFlame->setScale(0.5f, 0.5f, 0.5f);

    Texture2D& flameTex = ResourceManager::LoadTexture("flame", "textures/fire.png");
    Texture2D& noiseTex2 = ResourceManager::LoadTexture("noise2", NoiseTextureName2);
    mFlame->setTextureId(flameTex.ID);
    mFlame->setSecondaryTextureId(noiseTex2.ID);

    Shader& flameShader = ResourceManager::LoadShader("Flame");
    mFlame->setShader(&flameShader);
    flameShader.SetTexture("billboardTex", true, 0);
    flameShader.SetTexture("noiseTex",true, 1);

    // flame2
    mFlame2 = new Object3D();
    mFlame2->setMesh(trapezium);
    mFlame2->setPosition(glm::vec3(0.0f, 0.1f, 0.1f));
    mFlame2->setScale(0.1f, 0.1f, 0.1f);
    mFlame2->setTextureId(flameTex.ID);
    mFlame2->setSecondaryTextureId(noiseTex2.ID);
    mFlame2->setShader(&flameShader);

    // ====== SETUP LIGHTS ====== //
    glm::vec3 lightColor = glm::vec3(1, 0.7, 0);
    for (int i = 0; i < mLights.size(); i++) {
        // Shader emissive
        mLights[i]->setShader(&emissionShader);
        mLights[i]->setLight(lightColor, 1, 1.0, 0.1, 2);  // Solo asigna mPointLight, sin GL todavía

        // Shaders receptores
        mLights[i]->initLightUniformIntoShader(&litShader, i, true);
        // Cachear uniforms de light position, se usan mucho en el render
        std::string uniformName = "lights[" + std::to_string(i) + "].position";
        GLint positionUniform = glGetUniformLocation(litShader.getID(), uniformName.c_str());
        mLightPositionsUniforms.push_back(positionUniform);

    }
    // Light count en los receptores!
    litShader.SetInteger("lightCount", mLights.size(), true);
    smokeShader.SetInteger("lightCount", mLights.size() + mFakeSparks.size(), true);


    // ====== FAKE SPARKS ===== //
    for (int i = 0; i < FAKE_SPARKS_NUMBER; i++) {
        Spark* fakeSpark = new Spark();
        mFakeSparks.push_back(fakeSpark);
        mFakeSparks[i]->setMesh(sparkMesh);
        mFakeSparks[i]->setShader(&emissionShader);
    }

    // Distorsion Effect

    Shader& distortionShader = ResourceManager::LoadShader(DistorsionShaderName);
    mEffects = new PostProcessor(distortionShader, screenWidth, screenHeight);
    noiseTex.Bind();
    distortionShader.SetTexture("noiseTex", true);
    // mEffects->Confuse = true;
    mEffects->Distort = true;
    // ====== FINAL ERROR CHECK ======= //
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

    mEffects->BeginRender();

    // -------- Lights --------- //
    for (auto light : mLights) {
        light->render(dt, mCamera);
    }
    for (auto fakeSpark : mFakeSparks) {
        fakeSpark->render(dt, mCamera);
    }

    // --- Update lit shader --- //
    Shader& litShader = ResourceManager::GetShader(GroundShaderName);
    litShader.SetVector3f("viewPosition", mCamera->getPosition(), true);

    for (int i = 0; i < mLights.size(); i++) { // Update light positions
        // Legacy, no optimizado
        // mLights[i]->updateLightPositionIntoShader(&litShader, i, false);

        // Tiene sentido, pero no optimizado
        // std::string uniformName = "lights[" + std::to_string(i) + "].position";
        // litShader.SetVector3f(uniformName.c_str(), mLights[i]->getPosition());

        // Optimización (practicamente imperceptible)
        glm::vec3 lightPos = mLights[i]->getPosition();
        glUniform3f(mLightPositionsUniforms[i], lightPos.x, lightPos.y, lightPos.z);
    }
    // -------- Smoke --------- //
    Shader& smokeShader = ResourceManager::GetShader(SmokeShaderName);
    smokeShader.Use();
    std::vector<LightEmissor*> debugSparks = mFakeSparks;
    int lightCount = mLights.size();
    for (int i = 0; i < mLights.size() + mFakeSparks.size(); i++) {
        std::string uniformName = "lightPositions[" + std::to_string(i) + "]";

        LightEmissor* lightEmissor = mLights[i];
        if (i < lightCount) {
            smokeShader.SetVector3f(uniformName.c_str(), mLights[i]->getPosition());
        }
        else {
            smokeShader.SetVector3f(uniformName.c_str(), mFakeSparks[i - lightCount]->getPosition());
        }
        // glm::vec3 lightPos = mLights[i]->getPosition();
        // glUniform3f(mLightPositionsUniforms[i], lightPos.x, lightPos.y, lightPos.z);
    }

    mGround->render(dt, mCamera);

    ResourceManager::GetShader(SmokeShaderName).SetFloat("time", totalTime, true);
    mSmoke->render(dt, mCamera);

    Shader& flameShader = ResourceManager::GetShader(FlameShaderName);
    flameShader.SetFloat("time", totalTime, true);

    flameShader.SetFloat("density", 0.5, true);
    mFlame->render(dt, mCamera);
    flameShader.SetFloat("density", 1, true);
    // mFlame2->render(dt, mCamera);

    mEffects->EndRender();
    // Update world origin screen
    glm::vec4 origin = mCamera->getViewProjectionProductMatrix() * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    origin /= origin.w; // Perspective divide
    // Convertir de NDC [-1, 1] a Texture Coords [0, 1]
    glm::vec2 screenPos = glm::vec2(origin.x, origin.y) * 0.5f + 0.5f;
    mEffects->PostProcessingShader.SetVector2f("worldOriginScreen", screenPos, true);

    const float fireRadius = 0.5f;
    glm::vec4 edgeNDC = mCamera->getViewProjectionProductMatrix() * glm::vec4(fireRadius, 0.0f, 0.0f, 1.0f);
    edgeNDC /= edgeNDC.w;
    glm::vec2 edgeScreenPos = glm::vec2(edgeNDC.x, edgeNDC.y) * 0.5f + 0.5f;
    float dynamicRadius = glm::distance(screenPos, edgeScreenPos);
    mEffects->PostProcessingShader.SetFloat("distortionInfluence", dynamicRadius, true);
    //
    mEffects->Render(totalTime);


}

#define π 3.14159265 // Siempre deberias tener a pi preparado si estas en C++


void FireScene::Update(float dt) {
    totalTime += dt;

    for (int i = 0; i < mLights.size(); i++) {
        mLights[i]->update(dt); // Since they are sparks, update method is overridden
    }
    for (int i = 0; i < mFakeSparks.size(); i++) {
        mFakeSparks[i]->update(dt);
    }

    mCamera->turn(mouseDeltaX, mouseDeltaY, dt);
}

// Billboard optimization suggestion:
// glDepthMask(GL_FALSE);        // no escribe en depth buffer
// glEnable(GL_BLEND);
// glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
// ResourceManager::GetShader(SmokeShaderName).SetFloat("time", totalTime, true);
// mSmoke->render(dt, mCamera);
// glDepthMask(GL_TRUE);         // restaurar



