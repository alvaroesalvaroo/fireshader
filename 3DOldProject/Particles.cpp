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
#include "GLFW/glfw3.h"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

using namespace glm;

GLuint gVAO;
GLuint gVBO;
GLuint gEBO;
GLuint gTexture;
char const *gTextureFilename = "textures/smokeRGBA.png";
char const *gShaderName = "colorTextureMatrix";
Shader gShader;

int gUniformTransform; // For the particles (no tenemos que hacer esto)



#define NUM_PARTICLES 10

// Positions of particles (will be generated random and incremented by time)
float gXpos[NUM_PARTICLES];
float gYpos[NUM_PARTICLES];
vec2 gVelocity[NUM_PARTICLES];
vec2 gScale[NUM_PARTICLES];

vec3 gCameraPosition = vec3(0.0, 0.0, 2.0);



GLfloat gVertexData [28]=
{
    // x,y,			r,g,b			u,v(s,t)
    -0.5f,  0.5f,	1.0,0.0,0.0,	0.0,1.0,
    0.5f,  0.5f,	1.0,1.0,1.0,	1.0,1.0,
    0.5f, -0.5f,	1.0,1.0,1.0,	1.0,0.0,
    -0.5f, -0.5f,	1.0,1.0,1.0,	0.0,0.0
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

void initGeometry() {
    glClearColor(0.0, 0.0, 0.0, 1.0);
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
    // 3 attributes pointer: position (location=0, size = 2, offset =0)
    //                       color (location = 1, size = 3, offset = 2)
    //                       uv (location = 2, size = 2, offset = 5)
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)(5 * sizeof(GLfloat)));
    // Activate this attributes
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    // Create IBO
    glGenBuffers(1, &gEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(gIndexData), gIndexData, GL_STATIC_DRAW);
    ///////////////////////////////////////////////////////
    // Get uniforms
    gUniformTransform = glGetUniformLocation(gShader.getID(), "transform");


    // Clean VAO
    glBindVertexArray(0);

    // Create texture
    loadTexture();

    // Decide initial positions
    // Init X- Y positions, velocities and scales
    for (int i = 0; i < NUM_PARTICLES; i++) {
        gXpos[i] = ((rand() % 2000) - 1000) / 1000.0f;   // Random between -1 and 1
        gYpos[i] = ((rand() % 2000) - 1000) / 1000.0f;

        gVelocity[i] = vec2(((rand() % 2000) - 1000) / 1000.0f, ((rand() % 2000) - 1000) / 1000.0f);
        // Random between 0.1 and 1:
        float scaleRandomValue = ((rand() % 1000) + 100) / 1000.0f;
        gScale[i] = vec2(scaleRandomValue, scaleRandomValue);

    }

}

void render(double time, double deltaTime) {
    glClear(GL_COLOR_BUFFER_BIT);
    gShader.Use();

    // Enabling transparency
    // glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBindTexture(GL_TEXTURE_2D, gTexture);

    glBindVertexArray(gVAO);

    for (int i = 0; i < NUM_PARTICLES; i++) {
        mat4 transformMatrix = mat4();
        transformMatrix = translate(transformMatrix, vec3(gXpos[i], gYpos[i], 0.0f));
        transformMatrix = scale(transformMatrix, vec3(gScale[i].x, gScale[i].y, 1.0f));
        // Apply our mat4 variable to the uniform
        glUniformMatrix4fv(gUniformTransform, 1, GL_FALSE, value_ptr(transformMatrix));
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

    gCameraPosition = vec3(gCameraPosition.x, gCameraPosition.y, gCameraPosition.z += 1.0 * deltaTime);
}

