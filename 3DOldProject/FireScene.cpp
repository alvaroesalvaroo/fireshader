//
// Created by alv18 on 29/04/2025.
//

// Basic
#include <stdio.h>
// Own modules
#include "../Scene.h"
#include "../src/Backup/Shader.h"
// OpenGl
#include <glad/glad.h>

// Texture loader
#include "../src/Backup/stb_image.h"

// Maths
#include <cmath>
#include "glm.hpp"
#include "gtc/type_ptr.hpp"
#include "Objects/LightEmissorCube.h"
#include "Objects/LitCube.h"
#include "Objects/TexturedLitMesh.h"
#include "Objects/NormalMapMesh.h"
#include "Objects/Flame.h"
#include "Objects/TexturedLitPlane.h"


using namespace glm;

char const* GroundTextureFilename = "textures/rocky_gravel_diff_1k.jpg";
char const* GroundNormalTextureFilename = "textures/rocky_gravel_nor_gl_1k.jpg";
char const* SmokeTextureName = "textures/smokeRGBA.png";

char const* GroundMeshFilename = "mesh/CurvedPlane2.obj";
char const* FlameMeshFilename = "mesh/Llama";
char const* QuadFilename = "mesh/Quad.obj";

char const* GroundShaderName = "LitTexturedMatrix";


char const* FlameShaderName = "Flame";


LightEmissorCube* gLightEmissor = new LightEmissorCube();
NormalMapMesh* gGround = new NormalMapMesh();
Flame* gFlame = new Flame();
TexturedLitMesh* gTexturedLitMesh = new TexturedLitMesh();

TexturedLitPlane* gSmokeAttemp = new TexturedLitPlane();
NormalMapMesh* gSmoke = new NormalMapMesh();

glm::vec3 gLightColor = glm::vec3(1.0f, 0.3f, 0.3f);

void initScene(const int screenWidth, const int screenHeight) {
  printf("Init FireScene\n");
    glClearColor(0.0, 0.0, 0.3, 1.0);

    // A light emissor
    gLightEmissor->generateCube(0.1f);    // Optional to see light position
    gLightEmissor->initKnownShader();

    // Decide light position and color
    //glm::vec3 someColor = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 lightPosition = glm::vec3(1.0f, -0.0f, -1.0f);

    // Point Light
    gLightEmissor->setLightColor(gLightColor);
    gLightEmissor->setPosition(lightPosition);

    // Ground
    gGround->loadMeshFromFile(GroundMeshFilename);
    gGround->setIllumination(gLightEmissor->getPointLight(), GroundShaderName);
    gGround->loadTextureFromFile(GroundTextureFilename, 1);
    gGround->loadNormalTexture(GroundNormalTextureFilename);
    gGround->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));

    gSmoke->loadMeshFromFile(QuadFilename);
    gSmoke->setIllumination(gLightEmissor->getPointLight(), GroundShaderName);
    //gSmoke->loadSmokeTexture(SmokeTextureName);
    gSmoke->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));

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

    printf("Initialization finished. Start rendering\n");

    // Handly debug
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}



void render(double time, double deltaTime) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    gLightEmissor->render(time, deltaTime, gCamera);
    gSmokeAttemp->render(time, deltaTime, gCamera);

    gGround->updateLightPositions();
    gGround->render(time, deltaTime, gCamera);

    //gFlame->render(time, deltaTime, gCamera);
    gSmokeAttemp->updateLightPositions(gLightEmissor->getPointLight()->position);
    gSmokeAttemp->render(time, deltaTime, gCamera);
}

void updateGeometry(double time, double deltaTime) {
    glm::vec3 lightPosition = glm::vec3(0.3f + cos(time), 0.5, 2 * sin(time));
    gLightEmissor->setPosition(lightPosition);


}

