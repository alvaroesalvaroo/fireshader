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

char const* GroundTextureFilename = "textures/rocky_gravel_diff_1k.jpg";
char const* GroundNormalTextureFilename = "textures/rocky_gravel_nor_gl_1k.jpg";
char const* SmokeTextureName = "textures/smokeRGBA.png";

char const* GroundMeshFilename = "mesh/CurvedPlane2.obj";
char const* FlameMeshFilename = "mesh/Llama";
char const* QuadFilename = "mesh/Quad.obj";

char const* GroundShaderName = "NormalmapMultilit";

char const* FlameShaderName = "Flame";


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

#define SPARKS_NUMBER 10

void FireScene::Init() {

    // Handly debug
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    Mesh* cubeMesh = new Mesh();
    cubeMesh->createCubeMeshWithNoEBO(0.01);
    // TWO LIGHTS

    for (int i = 0; i < SPARKS_NUMBER; i++) {
        Spark* spark = new Spark();
        mLights.push_back(spark);

    }
    // LightEmissor* lightEmissor = new LightEmissor();
    // LightEmissor* lightEmissor2 = new LightEmissor();
    // mLights.push_back(lightEmissor);
    // mLights.push_back(lightEmissor2);

    GLenum err = glGetError();

    // Decide light position and color
    //glm::vec3 someColor = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 lightColor = glm::vec3(1, 0.7, 0);


    // Ground
    Mesh* groundMesh = new Mesh();
    groundMesh->loadMeshFromFile(GroundMeshFilename);
    mGround->mMesh = groundMesh;
    err = glGetError();

    Shader& litShader = ResourceManager::LoadShader(GroundShaderName);

    mGround->setShader(&litShader);

    // mGround->setIllumination(mLightEmissor->getPointLight(), GroundShaderName);
    err = glGetError();

    // mGround->loadTextureFromFile(GroundTextureFilename, 1);
    Texture2D& groundTex = ResourceManager::LoadTexture("ground", GroundTextureFilename);

    litShader.SetTexture("textura", true, 0);
    litShader.SetVector3f("viewPosition", mCamera->getPosition());

    Shader &emissionShader = ResourceManager::LoadShader("LightEmissor");
    // Point Lights
    for (int i = 0; i < mLights.size(); i++) {
        mLights[i]->mMesh = cubeMesh;
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



    // mGround->loadNormalTexture(GroundNormalTextureFilename);
    mGround->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    mGround->setTextureId(groundTex.ID);

    Material* mat = new Material(0.1f, 0.8f, 0.4f);
    mat->updateMaterialToShader(&ResourceManager::GetShader(GroundShaderName));


    // gSmoke->loadMeshFromFile(QuadFilename);
    // gSmoke->setIllumination(gLightEmissor->getPointLight(), GroundShaderName);
    //gSmoke->loadSmokeTexture(SmokeTextureName);
    // gSmoke->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));

    /*
    // Flames
    gFlame->loadFlamesData(FlameMeshFilename, 3);
    //gFlame->initShader(FlameShaderName);
    gFlame->setPosition(glm::vec3(0.0f, 1.0f, 1.0f));
    gFlame->initShader("LightEmissor");*/
    /*
    // Smoke with plane
    gSmokeAttemp->generateCube(1.0f);
    gSmokeAttemp->initKnownShader();
    gSmokeAttemp->loadSmokeTexture(SmokeTextureName);  // returns an id if needed
    gSmokeAttemp->setIlumination(gLightEmissor->getPointLight()->diffuse, gLightEmissor->getPointLight()->position);
    */

    err = glGetError();
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

    //gFlame->render(time, deltaTime, mCamera);
    // gSmokeAttemp->updateLightPositions(mLightEmissor->getPointLight()->position);
    // gSmokeAttemp->render(dt, mCamera);
}



#define PI 3.14159265
float totalTime = 0;

void FireScene::Update(float dt) {
    totalTime += dt;

    for (int i = 0; i < mLights.size(); i++) {
        mLights[i]->update(dt); // Since theyre sparks, update method is override
    }

    mCamera->turn(mouseDeltaX, mouseDeltaY, dt);
}



