//
// Created by alv18 on 04/05/2025.
//

#ifndef LIGHTEMISSORCUBE_H
#define LIGHTEMISSORCUBE_H
#include "Object3D.h"


struct PointLight { // Same structure as in the shader
    // glm::vec3 position;  // Will match object position
    glm::vec3 color;

    float intensity = 1.0f;

    // Attenuation
    float attConstant = 1.0f;
    float attLinear = 0.1f;
    float attQuadratic = 0.04f;

};
class LightEmissor : public Object3D{
public:

    LightEmissor() : Object3D() {
    }

    // void initEmissionShader();

    void initLightUniformIntoShader(Shader *shader, bool useShader);
    void initLightUniformIntoShader(Shader *shader, int index, bool useShader);

    void updateLightPositionIntoShader(Shader *shader, bool useShader);
    void updateLightPositionIntoShader(Shader *shader, int index, bool useShader);

    void setLight(glm::vec3 color, float intensity = 1.0f, float constant = 1.f, float linear = 0.1f, float quadratic = 0.04f);
    void setLightColor(glm::vec3 lightColor);

    struct PointLight* getPointLight();
private:
    // std::string mShaderName = "LightEmissor";

    struct PointLight mPointLight;

};



#endif //LIGHTEMISSORCUBE_H
