//
// Created by alv18 on 04/05/2025.
//

#include "LightEmissor.h"

#include "Camera3D.h"
#include "Camera3D.h"

void LightEmissor::initLightEmissorShader() {
    // This method init a object who already has an atribute mShaderName

    Object3D::initShader(mShaderName);
    mShader->Use();

    // We also assign extra uniform: the light color

    mLightColorUniform = glGetUniformLocation(mShader->getID(), "lightColor");

    glUniform3f(mLightColorUniform, mPointLight.color.x, mPointLight.color.y, mPointLight.color.z);

}

void LightEmissor::updateLightUniformIntoShader(Shader* shader, bool useShader) {
    if (useShader) shader->Use();

    shader->SetVector3f("light.color", mPointLight.color);
    shader->SetVector3f("light.position", mPosition);
    shader->SetFloat("light.intensity", mPointLight.intensity);
    shader->SetFloat("light.attConstant", mPointLight.attConstant);
    shader->SetFloat("light.attLinear",    mPointLight.attLinear);
    shader->SetFloat("light.attQuadratic", mPointLight.attQuadratic);
}

void LightEmissor::updateLightPositionIntoShader(Shader* shader, bool useShader) {
    if (useShader) shader->Use();
    shader->SetVector3f("light.position", mPosition);
}

void LightEmissor::setLight(glm::vec3 color, float intensity, float constant, float linear, float quadratic) {
    // Assign structure
    mPointLight.color = color;
    mPointLight.intensity = intensity;
    mPointLight.attConstant = constant;
    mPointLight.attLinear = linear;
    mPointLight.attQuadratic = quadratic;

    // Update ONLY THE LIGHT EMISSOR SHADER (color). We have to update the light receptors in their objects
    mShader->Use();
    // In own shader, update light color
    glUniform3f(mLightColorUniform, color.x, color.y, color.z);
}

void LightEmissor::setLightColor(glm::vec3 lightColor) {
    mPointLight.color = lightColor;
    // In own shader, update light color
    glUniform3f(mLightColorUniform, mPointLight.color.x, mPointLight.color.y, mPointLight.color.z);
}

PointLight* LightEmissor::getPointLight() {
    return &mPointLight;
}





