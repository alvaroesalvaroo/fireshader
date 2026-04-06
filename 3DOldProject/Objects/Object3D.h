//
// Created by alv18 on 03/05/2025.
//

#ifndef OBJECT3D_H
#define OBJECT3D_H
#include <vector>
#include <string>

#include <glad/glad.h>
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "Shader.h"
#include "../../src/Camera3D.h"

struct PointLight { // Same structure as in the shader
    glm::vec3 position;  // Will match object position
    float intensity = 2.0f;
    float constant;
    float linear;
    float quadratic;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};

class Object3D {
public:
    Object3D();
    virtual ~Object3D();
    int loadTextureFromFile(char const* filename, int textureIndex);
    void setTextureId(int textureId) {mTexture = textureId;}
    virtual void generateCube(float size);

    virtual void render(double time, double deltaTime, Camera3D *camera);
    virtual void prepareVAO();  // maybe private?
    void clearGPU();

    virtual void update(double time, double deltaTime); // The goal is not to use it (very much)
    virtual void setPosition(glm::vec3 position);
    void setRotation(glm::vec3 rotationAxis, float angle);
    void setScale(glm::vec3 scale);

    Shader * getShader();   // shader should be managed by object himself. However, there is an option for setShader
    virtual void initShader(char const *shaderName);

    virtual void setShader(Shader* shader);

protected:
    GLuint mVAO;
    GLuint mVBO;
    GLuint mEBO;
    GLuint mTexture;
    int mTextureIndex;

    bool mIsTransparent;

    std::vector<GLfloat> mVertexData;
    std::vector<GLuint> mIndexData;


    int mArraysToDraw;  // 36 for cubes, 6 for planes
    bool mUseEBO = false;

    Shader* mShader;

    GLint mUniformModel;
    GLint mUniformProjection;
    GLint mUniformView;

    glm::vec3 mPosition;
    glm::vec3 mRotationAxis;
    float mRotationAngle;
    glm::vec3 mScale;

};



#endif //OBJECT3D_H
