//
// Created by alv18 on 29/04/2025.
//

// Basic


#include <stdio.h>
// Own modules
#include "LightTestScene.h"
#include "Shader.h"
// OpenGl
#include <glad/glad.h>

// Texture loader
#include "stb_image.h"

// Maths
#include <cmath>
#include "glm.hpp"
#include "gtc/type_ptr.hpp"
#include "LightEmissor.h"
#include "Material.h"
#include "ResourceManager.h"
#include "GLFW/glfw3.h"


using namespace glm;

char const* GroundTextureFilename = "textures/rocky_gravel_diff_1k.jpg";
char const* GroundNormalTextureFilename = "textures/rocky_gravel_nor_gl_1k.jpg";
char const* SmokeTextureName = "textures/smokeRGBA.png";

char const* GroundMeshFilename = "mesh/CurvedPlane2.obj";
char const* FlameMeshFilename = "mesh/Llama";
char const* QuadFilename = "mesh/Quad.obj";

char const* GroundShaderName = "LitMaterial";

char const* FlameShaderName = "Flame";




LightTestScene::LightTestScene(int width, int height) : ::Scene(width, height) {
    // mLitCube = new Object3D();
    mTexturedLitCube = new Object3D();
    mLightEmissor = new LightEmissor();
    mCamera = new Camera3D(2.f, 5.f, 45.0f);
    mGround = new Object3D();
}

LightTestScene::~LightTestScene() {
}

void LightTestScene::Init() {

    Mesh* cubeMesh = new Mesh();
    cubeMesh->createCubeMeshWithNoEBO(0.1);
    // A light emissor
    mLightEmissor->mMesh = cubeMesh;  // Optional to see light position
    mLightEmissor->initLightEmissorShader();

    // Decide light position and color
    //glm::vec3 someColor = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 lightPosition = glm::vec3(1.0f, -0.0f, -1.0f);
    glm::vec3 lightColor = glm::vec3(1.0f, 0.3f, 0.3f);

    // Point Light
    mLightEmissor->setLightColor(lightColor);
    mLightEmissor->setPosition(lightPosition);

    // Ground
    Mesh* groundMesh = new Mesh();
    groundMesh->loadMeshFromFile(GroundMeshFilename);
    mGround->mMesh = groundMesh;
    mGround->initShader(GroundShaderName);
    // mGround->setIllumination(mLightEmissor->getPointLight(), GroundShaderName);
    // mGround->loadTextureFromFile(GroundTextureFilename, 1);
    Texture2D& groundT = ResourceManager::LoadTexture("ground", GroundTextureFilename);
    ResourceManager::GetShader(GroundShaderName).SetTexture("textura", true, 0);
    ResourceManager::GetShader(GroundShaderName).SetVector3f("viewPosition", mCamera->getPosition());
    ResourceManager::GetShader(GroundShaderName).SetVector3f("lightColor", lightColor);
    ResourceManager::GetShader(GroundShaderName).SetVector3f("lightPosition", lightPosition);
    // mGround->loadNormalTexture(GroundNormalTextureFilename);
    mGround->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    mGround->setTextureId(groundT.ID);

    Material* mat = new Material(0.1f, 0.8f, 0.4f);
    mat->updateMaterialToShader(&ResourceManager::GetShader(GroundShaderName));


    printf("Initialization finished. Start rendering\n");

    // Handly debug
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void LightTestScene::ProcessInput(float dt) {
    bool moveArray[6] = { false };

    if (this->Keys[GLFW_KEY_W]) moveArray[0] = true; // Forward
    if (this->Keys[GLFW_KEY_S]) moveArray[1] = true; // Backward
    if (this->Keys[GLFW_KEY_A]) moveArray[2] = true; // Left
    if (this->Keys[GLFW_KEY_D]) moveArray[3] = true; // Right
    if (this->Keys[GLFW_KEY_E]) moveArray[4] = true; // Up
    if (this->Keys[GLFW_KEY_Q]) moveArray[5] = true; // Down

    mCamera->updatePosition(moveArray, dt);
}


void LightTestScene::Render(float dt) {
    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // glEnable(GL_DEPTH_TEST);

    mLightEmissor->render(dt, mCamera);
    // gSmokeAttemp->render(dt, mCamera);

    ResourceManager::GetShader(GroundShaderName).SetVector3f("viewPosition", mCamera->getPosition(), true);
    ResourceManager::GetShader(GroundShaderName).SetVector3f("lightPosition", mLightEmissor->getPosition(), true);

    mGround->render(dt, mCamera);

    //gFlame->render(time, deltaTime, mCamera);
    // gSmokeAttemp->updateLightPositions(mLightEmissor->getPointLight()->position);
    // gSmokeAttemp->render(dt, mCamera);
}

float totalTime = 0;
void LightTestScene::Update(float dt) {
    totalTime += dt;
    glm::vec3 lightPosition = glm::vec3(0.3f + cos(totalTime), 0.5, 2 * sin(totalTime));
    mLightEmissor->setPosition(lightPosition);

    mCamera->turn(mouseDeltaX, mouseDeltaY, dt);

}

