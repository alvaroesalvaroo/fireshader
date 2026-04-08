//
// Created by alv18 on 04/05/2025.
//

#include "LitCube.h"

#include <iostream>


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

    // Uniforms: lightColor, objectColor, lightPosition, viewPosition
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
