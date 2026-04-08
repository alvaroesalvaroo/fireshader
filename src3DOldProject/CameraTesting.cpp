//
// Created by alv18 on 29/04/2025.
//

// Basic
#include <stdio.h>
// Own modules
#include "Geometry.h"
#include "Shader.h"
// OpenGl
#include <glad/glad.h>

// Texture loader
#include "stb_image.h"

// Maths
#include <cmath>
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
using namespace glm;

GLuint gVAO;
GLuint gVBO;
GLuint gEBO;
GLuint gTexture;
char const *gTextureFilename = "textures/smokeRGBA.png";
char const *gShaderName = "textureMatrix";
Shader gShader;


int gUniformModel;
int gUniformView;
int gUniformProjection;




#define NUM_PARTICLES 10

// Positions of particles (will be generated random and incremented by time)
float gXpos[NUM_PARTICLES];
float gYpos[NUM_PARTICLES];
glm::vec2 gVelocity[NUM_PARTICLES];
glm::vec2 gScale[NUM_PARTICLES];





GLfloat gVertexData [28]=
{
    // x,y, z   		u,v(s,t)
    -0.5f,  0.5f, 0.0f, 	0.0,1.0,
    0.5f,  0.5f, 0.0f, 		1.0,1.0,
    0.5f, -0.5f, 0.0f, 		1.0,0.0,
    -0.5f, -0.5f, 0.0f, 	0.0,0.0
};

GLuint gIndexData[6] =
{
// triangle1    //triangle2
    0, 1, 2,   0, 2, 3
};


void loadTexture() {
    glGenTextures(1, &gTexture);
    glBindTexture(GL_TEXTURE_2D, gTexture);
    // Load file
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *pixels = stbi_load(gTextureFilename, &width, &height, &nrChannels, 0);
    if (!pixels) {
        printf("Failed to load texture: %s\n", stbi_failure_reason());
        return;
    }

    printf("Texture loaded with number of channels = %d\n", nrChannels);

    GLint Mode = (nrChannels == 3) ? GL_RGB : GL_RGBA;


                //type, mipmaplevel, internalformat, width, height, border (always 0), format, type, pixelsdata
    glTexImage2D(GL_TEXTURE_2D, 0, Mode, width, height, 0, Mode, GL_UNSIGNED_BYTE, pixels);
    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        printf("ERROR on texture: %d\n", err);
    }
    glGenerateMipmap(GL_TEXTURE_2D);


    // Wrapping mode in both direction
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Scaling modes
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Cleaning
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(pixels);

}


void initGeometry(const int screenWidth, const int screenHeight) {

    glClearColor(0.0, 0.0, 0.0, 1.0);
    gCamera->setScreenSize(screenWidth, screenHeight);
    gCamera->setPosition(0.0f, 0.0f, 5.0f);
    gShader.init(gShaderName);
    printf("Initializing geometry texture\n");
    // Create VAO
    glGenVertexArrays(1, &gVAO);
    glBindVertexArray(gVAO);

    // Create VBO
    glGenBuffers(1, &gVBO);
    glBindBuffer(GL_ARRAY_BUFFER, gVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(gVertexData), gVertexData, GL_STATIC_DRAW);
    //////////////////////////////////////////////////////////////////////
    // 2 attributes pointer: LVertexpos (location=0, size = 3, offset =0)
    //                       texCoor (location = 1, size = 2, offset = 3)

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    // glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)(5 * sizeof(GLfloat)));
    // Activate this attributes
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    // glEnableVertexAttribArray(2);

    // Create IBO
    glGenBuffers(1, &gEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(gIndexData), gIndexData, GL_STATIC_DRAW);
    ///////////////////////////////////////////////////////
    // Get uniforms
    gUniformModel = glGetUniformLocation(gShader.getID(), "model");
    gUniformView = glGetUniformLocation(gShader.getID(), "view");
    gUniformProjection = glGetUniformLocation(gShader.getID(), "projection");


    // Clean VAO
    glBindVertexArray(0);

    // Create texture
    loadTexture();

    // Decide initial positions
    // Init X- Y positions, velocities and scales
    for (int i = 0; i < NUM_PARTICLES; i++) {
        gXpos[i] = ((rand() % 2000) - 1000) / 1000.0f;   // Random between -1 and 1
        gYpos[i] = ((rand() % 2000) - 1000) / 1000.0f;

        gVelocity[i] = glm::vec2(((rand() % 2000) - 1000) / 1000.0f, ((rand() % 2000) - 1000) / 1000.0f);
        // Random between 0.1 and 1:
        float scaleRandomValue = ((rand() % 1000) + 100) / 1000.0f;
        gScale[i] = glm::vec2(scaleRandomValue, scaleRandomValue);

    }

}

void render(double time, double deltaTime) {
    glClear(GL_COLOR_BUFFER_BIT);
    gShader.Use();

    // Enabling transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBindTexture(GL_TEXTURE_2D, gTexture);

    glBindVertexArray(gVAO);

    for (int i = 0; i < NUM_PARTICLES; i++) {
        mat4 transformMatrix = mat4();
        transformMatrix = glm::translate(transformMatrix, glm::vec3(0.0, -0.5, 0.0));
        transformMatrix = translate(transformMatrix, glm::vec3(gXpos[i], gYpos[i], 0.0f));
        transformMatrix = scale(transformMatrix, glm::vec3(gScale[i].x, gScale[i].y, 1.0f));
        // "rotate" can be added


        // Apply our variables to the uniforms
        glUniformMatrix4fv(gUniformModel, 1, GL_FALSE, value_ptr(transformMatrix));
        gCamera->updateViewUniform(gUniformView);
        gCamera->updateProjectionUniform(gUniformProjection);

                        // mode, number of vertex, tipe of indexes, offset in IBO
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

    // Clean
    glBindVertexArray(NULL);
    glUseProgram(NULL);

    // We do not clean texture if we don't need to: glBindTexture(GL_TEXTURE_2D, 0);
}

void updateGeometry(double time, double deltaTime) {

    for (int i = 0; i < NUM_PARTICLES; i++) {
        gXpos[i] += deltaTime * gVelocity[i].x;
        if (gXpos[i] > 1 || gXpos[i] < -1) {
            gVelocity[i].x = -gVelocity[i].x;
        }
        gYpos[i] += deltaTime * gVelocity[i].y;
        if (gYpos[i] > 1 || gYpos[i] < -1) {
            gVelocity[i].y = -gVelocity[i].y;
        }
        // Scale performed on render on shader
    }

}

