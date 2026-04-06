//
// Created by alv18 on 04/05/2025.
//

#include "TexturedUnlitCube.h"



void TexturedUnlitCube::generateCube(float size) {

    // Generate vertex and uv data

    // VBO data
    int dataSize = 120; // I would never have thought a cube was made out of no many numbers
    mUseEBO = true; // Some problem with uvs, i will still work on it if needed
    float vertexData[dataSize] = {
        // x, y, z,              u, v
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,   // 0
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,   // 1
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,   // 2
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,   // 3

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    };

    //EBO data
    GLuint indexData[36] =
    {
        // Cara frontal
        0, 1, 2,  2, 3, 0,
        // Cara trasera
        4, 5, 6,  6, 7, 4,
        // Cara derecha
        1, 2, 6,  6, 5, 1,
        // Cara izquierda
        0, 3, 7,  7, 4, 0,
        // Cara superior
        3, 2, 6,  6, 7, 3,
        // Cara inferior
        0, 1, 5,  5, 4, 0
    };
    mArraysToDraw = dataSize / 6;

    mVertexData.clear();
    mIndexData.clear();
    for (int i = 0; i < dataSize; i++) {
        mVertexData.push_back(vertexData[i] * size);
    }
    for (unsigned int i : indexData) {
        mIndexData.push_back(i);
    }

    prepareVAO();
    printf("Created textured unlit cube of size %f\n", size);
}

void TexturedUnlitCube::initKnownShader() {
    Object3D::initShader(mShaderName);
}

void TexturedUnlitCube::prepareVAO() {
    // Since this is a textured unlit cube, we can define here the shader name


    printf("preparing VAO\n");

    glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO);

    //Create VBO
    glGenBuffers(1, &mVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferData(GL_ARRAY_BUFFER, mVertexData.size() * sizeof(GLfloat), mVertexData.data(), GL_STATIC_DRAW);
    GLenum error;
    if ((error = glGetError()) != GL_NO_ERROR) {
        printf("ERROR buferring data: %d\n", error);
    }
    //////////////////////////////////////////////////////////////////////
    // Assuming our shader will have 2 attributes pointer:
    // layout (location = 0) in vec3 LVertexPos; -> (location=0, size = 3, offset =0)
    // layout (location = 1) in vec2 texCoor;   ->  (location = 1, size = 2, offset = 3)
    // with a stride of 5 floats

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));

    glEnableVertexAttribArray(0); // LVertexPos
    glEnableVertexAttribArray(1); // textCoor


    //Create IBO
    glGenBuffers(1, &mEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndexData.size() * sizeof (GLuint), mIndexData.data(), GL_STATIC_DRAW);
    glBindVertexArray(0);

    if (mVAO == 0 || mVBO == 0 || mEBO == 0) {
        printf("Error: VAO (%u), VBO (%u) o EBO (%u) no fueron creados correctamente.\n", mVAO, mVBO, mEBO);
    }

}


void TexturedUnlitCube::render(double time, double deltaTime, Camera3D *camera) {
    Object3D::render(time, deltaTime, camera);
}
