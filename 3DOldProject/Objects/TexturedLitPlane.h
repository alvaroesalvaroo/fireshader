//
// Created by alv18 on 04/05/2025.
//

#ifndef TexturedLitCube_H
#define TexturedLitCube_H
#include "Object3D.h"


class TexturedLitPlane : public Object3D{
public:
    TexturedLitPlane() : Object3D() {
        printf("Initializing light emissor cube\n");
        mLightColorUniform = -1;
        mLightPositionUniform = -1;
        mViewPositionUniform = -1;
    }
    // void generateCube(float size) override;
    void render(double time, double deltaTime, Camera3D *camera) override;
    // void prepareVAO() override;
    void initKnownShader();
    void generateCube(float size) override;
    void prepareVAO();
    void setObjectColor(glm::vec3 color);
    void setIlumination(glm::vec3 lightColor, glm::vec3 lightPosition);
    void updateLightPositions(glm::vec3 lightPosition);
    int loadSmokeTexture(char const* filename);
private:

    char const *mShaderName = "LitTexturedMatrix";

    glm::vec3 mLightColor = glm::vec3(1.0, 0.8, 0.5);
    glm::vec3 mLightPosition = glm::vec3(1.0f, 2.0f, -1.0f);
    glm::vec3 mObjectColor = glm::vec3(0.0, 1.0, 0.0);
    int mLightColorUniform;
    int mLightPositionUniform;
    // int mObjectColorUniform; // in case we want to give some base color
    int mViewPositionUniform;
};



#endif //TexturedLitCube_H
