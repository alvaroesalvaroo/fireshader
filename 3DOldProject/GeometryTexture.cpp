//
// Created by alv18 on 29/04/2025.
//

#include "Geometry.h"

#include <glad/glad.h>
#include "Shader.h"
#include <cmath>
#include <stdio.h>
#include "stb_image.h"
GLuint gVAO;
GLuint gVBO;
GLuint gEBO;
GLuint gTexture;
char const *gTextureFilename = "textures/smokeRGBA.png";
char const *gShaderName = "colorTexture";

GLfloat gVertexData [28]=
{
    // x,y,			r,g,b			u,v(s,t)
    -0.5f,  0.5f,	1.0,1.0,1.0,	0.0,1.0,
    0.5f,  0.5f,	1.0,1.0,1.0,	1.0,1.0,
    0.5f, -0.5f,	1.0,1.0,1.0,	1.0,0.0,
    -0.5f, -0.5f,	1.0,1.0,1.0,	0.0,0.0
};

GLuint gIndexData[6] =
{
// triangle1    //triangle2
    0, 1, 2,   0, 2, 3
};

Shader colorTextureShader;
void loadTexture() {
    glGenTextures(1, &gTexture);
    glBindTexture(GL_TEXTURE_2D, gTexture);
    // Load file
    int width, height, nrChannels;
    unsigned char *pixels = stbi_load(gTextureFilename, &width, &height, &nrChannels, 0);
    if (!pixels) {
        printf("Failed to load texture: %s\n", stbi_failure_reason());
        return;
    }

    printf("Texture loaded with number of channels = %d\n", nrChannels);

    GLint Mode = GL_RGBA;

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
    colorTextureShader.init(gShaderName);
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
    // Clean VAO
    glBindVertexArray(0);

    // Create texture
    loadTexture();

}

void render(double time) {
    glClear(GL_COLOR_BUFFER_BIT);
    colorTextureShader.Use();   // Only calls gUse program

    // Called on init and then cleaned. Then, called here again
    glBindTexture(GL_TEXTURE_2D, gTexture);
    glBindVertexArray(gVAO);

                    // mode, number of vertex, tipe of indexes, offset in IBO
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

    // Clean
    glBindVertexArray(NULL);
    glUseProgram(NULL);

    // We do not clean texture if we dont need it: glBindTexture(GL_TEXTURE_2D, 0);
}

void updateGeometry(double time) {

}

