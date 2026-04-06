//
// Created by alv18 on 04/05/2025.
//

#include "LitCube.h"


void LitCube::render(double time, double deltaTime, Camera3D *camera) {
    mShader->Use();
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glm::vec3 viewPosition = camera->getPosition();
    glUniform3f(mViewPositionUniform, viewPosition.x, viewPosition.y, viewPosition.z);
    Object3D::render(time, deltaTime, camera);
}

void LitCube::initKnownShader() {
    Object3D::initShader(mShaderName);
    // Assign extra uniforms: the light color, the light position, and the object color
    mShader->Use();


    mLightColorUniform = glGetUniformLocation(mShader->getID(), "lightColor");
    glUniform3f(mLightColorUniform, mLightColor.x, mLightColor.y, mLightColor.z);

    mObjectColorUniform = glGetUniformLocation(mShader->getID(), "objectColor");
    glUniform3f(mObjectColorUniform, mObjectColor.x, mObjectColor.y, mObjectColor.z);

    mLightPositionUniform = glGetUniformLocation(mShader->getID(), "lightPosition");
    glUniform3f(mLightPositionUniform, mLightPosition.x, mLightPosition.y, mLightPosition.z);

    mViewPositionUniform = glGetUniformLocation(mShader->getID(), "viewPosition");

    printf("LitCube inited with shader %s. Assigned light color, object color, light position and view position uniforms\n", mShaderName);
    // Sera actualizada en render a la camera position

    /* Some extra debugging
    GLfloat lightColorCheck[3];
    glGetUniformfv(mShader->getID(), mLightColorUniform, lightColorCheck);
    printf("Uniform lightColor en GPU: (%f, %f, %f)\n", lightColorCheck[0], lightColorCheck[1], lightColorCheck[2]);
    */
}

void LitCube::generateCube(float size) {
    // LA DATA ES DIFERENTE PORQUE AHORA TIENE NORMALS
    // No tenemos UV, por lo que el stride es de 6
    int dataSize = 216;
    float vertices[dataSize] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    GLuint indexData[36] =
    {
        // Cara frontal
        0, 1, 2,  2, 3, 0,
        // Cara trasera
        4, 5, 6,  6, 7, 4,
        // Cara izquierda
        0, 3, 7,  7, 4, 0,
        // Cara derecha
        1, 2, 6,  6, 5, 1,
        // Cara superior
        3, 2, 6,  6, 7, 3,
        // Cara inferior
        0, 1, 5,  5, 4, 0
    };

    mVertexData.clear();
    mIndexData.clear();
    for (int i = 0; i < dataSize; i++) {
        mVertexData.push_back(vertices[i] * size);
    }
    for (unsigned int i : indexData) {
        mIndexData.push_back(i);
    }
    printf("Generated vertex for cube with size %f\n ", size);
    prepareVAO();
}

void LitCube::prepareVAO() {
    printf("preparing VAO for light Cube");

    glGenVertexArrays(1, &mVAO);

    //Create VBO
    glGenBuffers(1, &mVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferData(GL_ARRAY_BUFFER, mVertexData.size() * sizeof(GLfloat), mVertexData.data(), GL_STATIC_DRAW);

    glBindVertexArray(mVAO);
    // 2 ATRIBUTES: POSICION Y NORMALS, STRIDE 6

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // IBO skipped
    glGenBuffers(1, &mEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndexData.size() * sizeof (GLuint), mIndexData.data(), GL_STATIC_DRAW);
    glBindVertexArray(0);

    if (mVAO == 0 || mVBO == 0 || mEBO == 0) {
        printf("Error: VAO (%u), VBO (%u) o EBO (%u) no fueron creados correctamente.\n", mVAO, mVBO, mEBO);
    }


}

void LitCube::setObjectColor(glm::vec3 color) {
    mObjectColor = color;
    mShader->Use();
    glUniform3f(mObjectColorUniform, color.x, color.y, color.z);
}

void LitCube::setIlumination(glm::vec3 lightColor, glm::vec3 lightPosition) {
    mLightColor = lightColor;
    mShader->Use();
    glUniform3f(mLightColorUniform, lightColor.x, lightColor.y, lightColor.z);
    glUniform3f(mLightPositionUniform, lightPosition.x, lightPosition.y, lightPosition.z);
}
