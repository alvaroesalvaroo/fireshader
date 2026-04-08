//
// Created by alv18 on 04/05/2025.
//

#include "LightEmissor.h"

void LightEmissor::render(double time, double deltaTime, Camera3D *camera) {
    mShader->Use();
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    Object3D::render(time, deltaTime, camera);
}

void LightEmissor::initKnownShader() {
    // This method init a object who already has an atribute mShaderName

    Object3D::initShader(mShaderName);
    mShader->Use();

    // We also assign extra uniform: the light color

    mLightColorUniform = glGetUniformLocation(mShader->getID(), "lightColor");
    glUniform3f(mLightColorUniform, mPointLight.diffuse.x, mPointLight.diffuse.y, mPointLight.diffuse.z);

}



void LightEmissor::setLight(float constant, float linear, float quadratic, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) {
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

void LightEmissor::setPosition(glm::vec3 position) {
    // Also update struct position
    Object3D::setPosition(position);
    mPointLight.position = position;
}


struct PointLight* LightEmissor::getPointLight() {
    mPointLight.position = mPosition;
    return &mPointLight;
}





