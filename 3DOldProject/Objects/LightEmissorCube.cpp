//
// Created by alv18 on 04/05/2025.
//

#include "LightEmissorCube.h"

void LightEmissorCube::render(double time, double deltaTime, Camera3D *camera) {
    mShader->Use();
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    Object3D::render(time, deltaTime, camera);
}

void LightEmissorCube::initKnownShader() {
    // This method init a object who already has an atribute mShaderName

    Object3D::initShader(mShaderName);
    mShader->Use();

    // We also assign extra uniform: the light color

    mLightColorUniform = glGetUniformLocation(mShader->getID(), "lightColor");
    glUniform3f(mLightColorUniform, mPointLight.diffuse.x, mPointLight.diffuse.y, mPointLight.diffuse.z);

    printf("Inited light emissor cube and assigned ligthColor uniform with shader %s\n", mShaderName);

}

void LightEmissorCube::generateCube(float size) {
    Object3D::generateCube(size);
}

void LightEmissorCube::prepareVAO() {

    glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO);

    //Create VBO
    glGenBuffers(1, &mVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferData(GL_ARRAY_BUFFER, mVertexData.size() * sizeof(GLfloat), mVertexData.data(), GL_STATIC_DRAW);

    //////////////////////////////////////////////////////////////////////
    // Textured shader has 2 attributes pointer:
    // layout (location = 0) in vec3 LVertexPos; -> (location=0, size = 3, offset =0)
    // layout (location = 1) in vec2 texCoor;   ->  (location = 1, size = 2, offset = 3) // Prescindible en este caso
    // with a stride of 5 floats

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));

    glEnableVertexAttribArray(0); // LVertexPos
    glEnableVertexAttribArray(1); // textCoor
    mUseEBO = false;
    glGenBuffers(1, &mEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndexData.size() * sizeof (GLuint), mIndexData.data(), GL_STATIC_DRAW);
    glBindVertexArray(0);
}

void LightEmissorCube::setLight(float constant, float linear, float quadratic, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) {
    // Assign structure
    mPointLight.constant = constant;
    mPointLight.linear = linear;
    mPointLight.quadratic = quadratic;
    mPointLight.ambient = ambient;
    mPointLight.diffuse = diffuse;
    mPointLight.specular = specular;

    // Update ONLY THE LIGHT EMISSOR SHADER (color). We have to update the light receptors in their objects
    mShader->Use();
    // We will update using diffuse color since is the most relevant
    glUniform3f(mLightColorUniform, diffuse.x, diffuse.y, diffuse.z);
}

void LightEmissorCube::setPosition(glm::vec3 position) {
    // Also update struct position
    Object3D::setPosition(position);
    mPointLight.position = position;
}


struct PointLight* LightEmissorCube::getPointLight() {
    mPointLight.position = mPosition;
    return &mPointLight;
}





