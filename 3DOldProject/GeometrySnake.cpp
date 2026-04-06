//
// This module manages geometry.
// The goal is to make it independent of GLFW or SDL (not of glad)
//

#include <glad/glad.h>
#include "Shader.h"
#include <cmath>
#include <stdio.h>

#define NUM_OBJ 15
#define PI 3.141592f // Cambiar por libreria de math
GLuint gVAO[NUM_OBJ];
GLuint gVBO[NUM_OBJ];
GLuint gEBO[NUM_OBJ];

Shader coloredShader;
int gUniformIDOffset;

//VBO data
const float gTriangleSize = 0.8;
GLfloat baseVertexData[6] = // Single triangle = 3 vertices * 2 coordenates
{
    0.0f, 0.1f,
    0.1f, -0.1f,
    -0.1f, -0.1f
};
GLfloat gVertexData[NUM_OBJ][3 * 5];

void generateTriangles() {

    for (int i = 0; i < NUM_OBJ; i++) {
        GLfloat yOffset =  -0.8f + i * (1.6f / (NUM_OBJ - 1)); // From -0.8 to +0.8
        printf("yOffset: %f\n", yOffset);
        GLfloat t = static_cast<GLfloat>(i) / (NUM_OBJ - 1); // 0 para el mas bajo, 1 para el mas alto
        GLfloat r = 1.0 - t;
        GLfloat g = 0.0;
        GLfloat b = t;

        for (int j = 0; j < 3; j++) {   // Cada uno de los verticles
            int index = j*5;    // Cada vertice tiene 5 valores: x, y, r, g, b
            // posicion
            gVertexData[i][index] = baseVertexData[j * 2] * gTriangleSize;  // x
            gVertexData[i][index + 1] = baseVertexData[j * 2 + 1] * gTriangleSize + yOffset; // y
            // color
            gVertexData[i][index + 2] = r;
            gVertexData[i][index + 3] = g;
            gVertexData[i][index + 4] = b;
        }
    }
    for (int i = 0; i < NUM_OBJ; i++) {
        printf("Triangle %d:\n", (i+1));
        for (int j = 0; j < 3; j++) {
            printf("x: %f\n", gVertexData[i][j*5]);
            printf("y: %f\n", gVertexData[i][j*5 + 1]);
            printf("r: %f\n", gVertexData[i][j*5 + 2]);
            printf("g: %f\n", gVertexData[i][j*5 + 3]);
            printf("b: %f\n", gVertexData[i][j*5 + 4]);
            printf("\n");

        }
    }

}

//EBO data
GLuint gIndexData[NUM_OBJ][3];
void generateIndexData() {
    for (int i = 0; i < NUM_OBJ; i++) {
        gIndexData[i][0] = 0;
        gIndexData[i][1] = 1;
        gIndexData[i][2] = 2;
    }
}


void initGeometry() {
    generateTriangles();
    generateIndexData();
    coloredShader.init("colored");
    printf("Colored shader program id: %d", coloredShader.getID());
    for (int i = 0; i < NUM_OBJ; i++) {
        // Create VAO
        glGenVertexArrays(1, &gVAO[i]);
        glBindVertexArray(gVAO[i]);
        {
            //Create VBO
            glGenBuffers(1, &gVBO[i]);
            glBindBuffer(GL_ARRAY_BUFFER, gVBO[i]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(gVertexData[i]), gVertexData[i], GL_STATIC_DRAW);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), NULL);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(2 * sizeof (GLfloat)));
            //Create IBO
            glGenBuffers(1, &gEBO[i]);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gEBO[i]);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(gIndexData[i]), gIndexData[i], GL_STATIC_DRAW);


            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);

        }
        glBindVertexArray(0);
    }
    gUniformIDOffset = glGetUniformLocation(coloredShader.getID(), "offsetX");

}

void render(double time) {

    glClear(GL_COLOR_BUFFER_BIT);
    coloredShader.Use();
    float velocity = 3.0f;
    for (int i = 0; i < NUM_OBJ; i++) {
        float objectOffset = i * 3.1415928/NUM_OBJ;
        float offset = 0.2f*sin(velocity * time + objectOffset);
        glUniform1f(gUniformIDOffset, offset);
        //printf("Offset: %f\n", Offset);
        //Set VAO
        glBindVertexArray(gVAO[i]);
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        //Draw VAO
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, NULL);
    }

    //Disable VAO
    glBindVertexArray(0);
    //Unbind program
    glUseProgram(NULL);
}

void updateGeometry(double time) {}


