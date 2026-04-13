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

using namespace glm;

char const* GroundTextureFilename = "textures/rocky_gravel_diff_1k.jpg";
char const* GroundNormalTextureFilename = "textures/rocky_gravel_nor_gl_1k.jpg";
char const* SmokeTextureName = "textures/smokeRGBA.png";

char const* GroundMeshFilename = "mesh/CurvedPlane2.obj";
char const* FlameMeshFilename = "mesh/Llama";
char const* QuadFilename = "mesh/Quad.obj";

char const* GroundShaderName = "MultiLitMaterial";

char const* FlameShaderName = "Flame";


FireScene::FireScene(int width, int height) : ::Scene(width, height) {
    // mLitCube = new Object3D();
    mTexturedLitCube = new Object3D();
    mLightPositionsUniforms.clear();
    mLights.clear();
    mCamera = new Camera3D(2.f, 5.f, 45.0f);
    mGround = new Object3D();
}

FireScene::~FireScene() {
}

#define SPARKS_NUMBER 5
#define SPARKS_RANDOMNESS 12.0f
#define TIME_TO_UPDATE_SPARKS 1.f
float timeSinceUpdateSparks = 0;

void FireScene::Init() {

    // Handly debug
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    Mesh* cubeMesh = new Mesh();
    cubeMesh->createCubeMeshWithNoEBO(0.1);
    // TWO LIGHTS

    for (int i = 0; i < SPARKS_NUMBER; i++) {
        LightEmissor* lightEmissor = new LightEmissor();
        mLights.push_back(lightEmissor);
        Spark newSpark;
        glm::vec3 randomVel = glm::vec3(
        ((rand() / (float)RAND_MAX) * 2.0f - 1.0f) * SPARKS_RANDOMNESS,
        0.0f,
        ((rand() / (float)RAND_MAX) * 2.0f - 1.0f) * SPARKS_RANDOMNESS);
        newSpark.velocity = randomVel;
        mSparks.push_back(newSpark);

    }
    // LightEmissor* lightEmissor = new LightEmissor();
    // LightEmissor* lightEmissor2 = new LightEmissor();
    // mLights.push_back(lightEmissor);
    // mLights.push_back(lightEmissor2);

    GLenum err = glGetError();

    // Decide light position and color
    //glm::vec3 someColor = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 lightPosition = glm::vec3(1.0f, -0.0f, -1.0f);
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
        // 1. Datos puros, sin GL
        mLights[i]->mMesh = cubeMesh;
        mLights[i]->setPosition(lightPosition);
        // Shader emissive
        mLights[i]->setShader(&emissionShader);
        // Both emissive (color) and receptive (setups uniform)
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




void updateSpark(Spark &spark, float dt) {

    const float speedY        = 1.2f;
    const float noiseStrength = 800.f;
    const float damping       = 0.6f;
    const float maxY = 3.0f;

    spark.accTimer += dt;
    if (spark.accTimer >= spark.accUpdateRate) {
        spark.accTimer = 0;
        spark.accUpdateRate = 1.f + (rand() / (float)RAND_MAX);
        // std::cout << "update spark acc" << std::endl;

        glm::vec3 randomAccel = glm::vec3(
            ((rand() / (float)RAND_MAX) * 2.0f - 1.0f) * noiseStrength,
            0.0f,
            ((rand() / (float)RAND_MAX) * 2.0f - 1.0f) * noiseStrength
        );
        spark.acceleration = randomAccel;
    }

    spark.velocity.x = spark.velocity.x * damping + spark.acceleration.x * dt;
    spark.velocity.y = speedY;
    spark.velocity.z = spark.velocity.z * damping + spark.acceleration.z * dt;

    spark.position += spark.velocity * dt;
    spark.lifetime += dt;

    if (spark.position.y > maxY) {
        spark.position = glm::vec3(0.0f);
        spark.velocity = glm::vec3(rand() / (float)RAND_MAX, 0.0f, rand() / (float)RAND_MAX);
        spark.lifetime = 0.0f;
    }
}

#define PI 3.14159265
float totalTime = 0;

void FireScene::Update(float dt) {
    totalTime += dt;
    // glm::vec3 lightPosition = glm::vec3(0.3f + cos(totalTime), 0.5, 2 * sin(totalTime));
    // glm::vec3 lightPosition2 = glm::vec3(0.3f + cos(totalTime + PI), 0.5, 2 * sin(totalTime + PI));
    // mLights[0]->setPosition(lightPosition);
    // mLights[1]->setPosition(lightPosition2);


    // TODO: Chispas

    for (int i = 0; i < mSparks.size(); i++) {
        updateSpark(mSparks[i], dt);
        mLights[i]->setPosition(mSparks[i].position);
    }

    mCamera->turn(mouseDeltaX, mouseDeltaY, dt);
}



