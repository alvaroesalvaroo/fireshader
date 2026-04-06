//
// Created by alv18 on 04/05/2025.
//

#ifndef TexturedLitMesh_H
#define TexturedLitMesh_H
#include "Object3D.h"

class TexturedLitMesh : public Object3D{
public:
    TexturedLitMesh() : Object3D() {
        printf("Initializing light emissor cube\n");
        mViewPositionUniform = -1;
        mUVData = std::vector<GLfloat>();
        mNormalData = std::vector<GLfloat>();
        mVBO_Normal = -1;
        mVBO_UV = -1;
        mPointLight = nullptr;
    }
    // Override adds camera update
    void render(double time, double deltaTime, Camera3D *camera) override;

    void loadMeshFromFile(char const *filename);
    void prepareMeshData();
    void setObjectColor(glm::vec3 color);
    void setIllumination(struct PointLight *light, char const* shaderName);

    void updateLightPositions();

private:
    // LIGHTS (in future will be a list):
    struct PointLight *mPointLight;

    // Extra geometry
    GLuint mVBO_Normal;
    GLuint mVBO_UV;

    std::vector<GLfloat> mNormalData;
    std::vector<GLfloat> mUVData;

    int mViewPositionUniform;

    // in case we want to give some color
    // glm::vec3 mObjectColor = glm::vec3(0.0, 1.0, 0.0);
    // int mObjectColorUniform;

};



#endif //TexturedLitCube_H
