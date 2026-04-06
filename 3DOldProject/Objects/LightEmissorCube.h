//
// Created by alv18 on 04/05/2025.
//

#ifndef LIGHTEMISSORCUBE_H
#define LIGHTEMISSORCUBE_H
#include "Object3D.h"



class LightEmissorCube : public Object3D{
public:
    LightEmissorCube() : Object3D() {
        mLightColorUniform = -1;
        // Default light
        // mPointLight.intensity = 2.0f;
        // Attenuation
        mPointLight.constant = 1.0f;
        mPointLight.linear = 0.7f;  //0.7f
        mPointLight.quadratic = 1.8f;   //1.8f
        // Lights
        mPointLight.diffuse = glm::vec3(1.0, 1.0, 1.0);
        mPointLight.ambient = glm::vec3(1.0, 1.0, 1.0);
        mPointLight.specular = glm::vec3(1.0, 1.0, 1.0);
    }

    // void generateCube(float size) override;
    void render(double time, double deltaTime, Camera3D *camera) override;
    // void prepareVAO() override;
    void initKnownShader();
    void generateCube(float size) override;
    void prepareVAO() override;
    void setLight(float constant, float linear, float quadratic, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
    void setLightColor(glm::vec3 lightColor) {
        // Assign 3 ambient diffuse and specular colors to the same
        setLight(mPointLight.constant, mPointLight.linear, mPointLight.quadratic, lightColor, lightColor, lightColor);
    }
    void setPosition(glm::vec3 position) override;
    struct PointLight* getPointLight();
private:
    char const *mShaderName = "LightEmissor";
    int mLightColorUniform;

    struct PointLight mPointLight;

};



#endif //LIGHTEMISSORCUBE_H
