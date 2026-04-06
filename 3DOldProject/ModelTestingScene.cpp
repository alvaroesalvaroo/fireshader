//
// Created by alv18 on 29/04/2025.
// En esta escena observamos una fuente de luz dando vueltas alrededor de un terreno irregular.
// La luz ilumina los vértices del terreno y refleja en la dirección de la cámara de acuerdo al modelo ambient-diffuse-specular
//

// Basic
#include <stdio.h>
// Own modules
#include "../Scene.h"
#include "../src/Shader.h"
// OpenGl
#include <glad/glad.h>

// Texture loader
#include "stb_image.h"

// Maths
#include <cmath>
#include "glm.hpp"
#include "gtc/type_ptr.hpp"
#include "Objects/LightEmissorCube.h"
#include "Objects/LitCube.h"
#include "Objects/TexturedLitMesh.h"
#include "Objects/NormalMapMesh.h"

using namespace glm;


char const* TextureFilename = "textures/rocky_gravel_diff_1k.jpg";  // textures/rocky_gravel_diff_1k.jpg
char const* NormalFilename = "textures/rocky_gravel_nor_gl_1k.jpg";
char const* MeshFilename = "mesh/CurvedPlane2.obj";

char const* ShaderName = "LitTexturedMatrix";
LightEmissorCube *gLightEmissor = new LightEmissorCube();
NormalMapMesh *gTexturedLitMesh = new NormalMapMesh();


glm::vec3 gLightColor = glm::vec3(1.0f, 0.6f, 0.6f);

void initScene(const int screenWidth, const int screenHeight) {
    glClearColor(0.0, 0.0, 0.3, 1.0);

    // SETUP light emissor
    gLightEmissor->generateCube(0.1f);    // Optional to see light position
    gLightEmissor->initKnownShader();

    // Decide light initial position

    glm::vec3 lightPosition = glm::vec3(1.0f, -0.0f, -1.0f);

    gLightEmissor->setLightColor(gLightColor);
    gLightEmissor->setPosition(lightPosition);

    printf("CUBO NUEVO\n");
    // Configure lighting settigs for lit objects
    gTexturedLitMesh->loadMeshFromFile(MeshFilename);
    // This method also inits shader
    gTexturedLitMesh->setIllumination(gLightEmissor->getPointLight(), ShaderName);
    gTexturedLitMesh->loadTextureFromFile(TextureFilename, 3);
    gTexturedLitMesh->loadNormalTexture(NormalFilename);

    gTexturedLitMesh->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));

    printf("Initialization finished. Start rendering\n");

    // Handly debug
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}



void render(double time, double deltaTime) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    // Render light emissor
    gLightEmissor->render(time, deltaTime, gCamera);

    // Update light positions in others object shaders
    gTexturedLitMesh->updateLightPositions();
    gTexturedLitMesh->render(time, deltaTime, gCamera);

}

void updateGeometry(double time, double deltaTime) {
    // Move light emissor
    glm::vec3 lightPosition = glm::vec3(0.3f + cos(time), 0.5, 2 * sin(time));
    gLightEmissor->setPosition(lightPosition);

}

