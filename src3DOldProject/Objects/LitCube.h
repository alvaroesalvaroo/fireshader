//
// Created by alv18 on 04/05/2025.
//

#ifndef LITCUBE_H
#define LITCUBE_H
#include "../../src/Object3D.h"


class LitCube : public Object3D{
public:
    LitCube() : Object3D() {
        printf("Initializing light emissor cube\n");
        mLightColorUniform = -1;
    }
    // void generateCube(float size) override;
    void render(double time, double deltaTime, Camera3D *camera) override;
    // void prepareVAO() override;
    void initKnownShader();
    void generateCube(float size) override;
    void setObjectColor(glm::vec3 color);
    void setIlumination(glm::vec3 lightColor, glm::vec3 lightPosition);
private:
    std::string mShaderName = "LitColorMatrix";

    glm::vec3 mLightColor = glm::vec3(1.0, 0.8, 0.5);
    glm::vec3 mLightPosition = glm::vec3(1.0f, 2.0f, -1.0f);
    glm::vec3 mObjectColor = glm::vec3(0.0, 1.0, 0.0);
    int mLightColorUniform;
    int mLightPositionUniform;
    int mObjectColorUniform;

    int mViewPositionUniform;
};



#endif //LITCUBE_H
