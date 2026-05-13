
#include "LightEmissor.h"

#include <iostream>
#include <ostream>

void LightEmissor::initLightUniformIntoShader(Shader* shader, bool useShader) {
    if (useShader) shader->Use();

    shader->SetVector3f("light.color", mPointLight.color);
    shader->SetVector3f("light.position", mPosition);
    shader->SetFloat("light.intensity", mPointLight.intensity);
    shader->SetFloat("light.attConstant", mPointLight.attConstant);
    shader->SetFloat("light.attLinear",    mPointLight.attLinear);
    shader->SetFloat("light.attQuadratic", mPointLight.attQuadratic);
}


void LightEmissor::initLightUniformIntoShader(Shader* shader, int index, bool useShader) {
    if (useShader) shader->Use();

    std::string base = "lights[" + std::to_string(index) + "].";

    shader->SetVector3f((base + "color").c_str(),        mPointLight.color);
    shader->SetVector3f((base + "position").c_str(),     mPosition);
    shader->SetFloat(   (base + "intensity").c_str(),    mPointLight.intensity);
    shader->SetFloat(   (base + "attConstant").c_str(),  mPointLight.attConstant);
    shader->SetFloat(   (base + "attLinear").c_str(),    mPointLight.attLinear);
    shader->SetFloat(   (base + "attQuadratic").c_str(), mPointLight.attQuadratic);
}

void LightEmissor::updateLightPositionIntoShader(Shader* shader, bool useShader) {
    // If no index is passed, light array will be ignored, one light assumed
    if (useShader) shader->Use();
    shader->SetVector3f("light.position", mPosition);
}

void LightEmissor::updateLightPositionIntoShader(Shader *shader, int index, bool useShader) {
    std::cout << "Metodo deprecated: updateLightPositionIntoShader for multiple lights. " << std::endl;

    if (useShader) shader->Use();
    std::string name = "lights[" + std::to_string(index) + "].position";

    shader->SetVector3f(name.c_str(), mPosition);
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
    mShader->SetVector3f("lightColor", color);
}

void LightEmissor::setLightColor(glm::vec3 lightColor) {
    mPointLight.color = lightColor;
    // In own shader, update light color
    mShader->SetVector3f("lightColor", lightColor);

}

PointLight* LightEmissor::getPointLight() {
    return &mPointLight;
}





