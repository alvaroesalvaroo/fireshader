//
// This module manages geometry.
// The goal is to make it independent of GLFW or SDL (not of glad)
//

#include <glad/glad.h>
#include "Shader.h"
#include <cmath>
#include <stdio.h>

#define NUM_OBJ 2
GLuint gVAO[NUM_OBJ];
GLuint gVBO[NUM_OBJ];
GLuint gEBO[NUM_OBJ];

Shader coloredShader;
int UniformIDOffset;

//VBO data
GLfloat gVertexData[2][15] =
{ {
    // x,y, r,g,b
    -0.5f,  0.5f, 1.0, 0.0, 0.0,
     0.0f, -0.5f, 0.0, 1.0, 0.0,
    -1.0f, -0.5f, 0.0, 0.0, 1.0
}, {
    0.5f,  0.5f, 1.0, 1.0, 0.0,
    0.0f, -0.5f, 1.0, 0.0, 1.0,
    1.0f, -0.5f, 0.0, 1.0, 1.0
} };


//EBO data
GLuint gIndexData[2][3] =
{ {
    0, 1, 2
}, {
    0, 1, 2
} };

void initGeometry() {
    coloredShader.init("colored");
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
    UniformIDOffset = glGetUniformLocation(coloredShader.getID(), "offsetX");
}

void render(double time) {
    glClear(GL_COLOR_BUFFER_BIT);
    coloredShader.Use();
    //Move figure

    float Offset = 0.2f*sin(((time / 2000))*3.141592f / 180.0f);
    glUniform1f(UniformIDOffset, Offset );

    //Set VAO
    glBindVertexArray(gVAO[0]);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //Draw VAO
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, NULL);

    //Set VAO
    glBindVertexArray(gVAO[1]);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //Draw VAO
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, NULL);

    //Disable VAO
    glBindVertexArray(0);
    //Unbind program
    glUseProgram(NULL);
}

void updateGeometry(double time) {}


