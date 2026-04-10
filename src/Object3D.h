//
// Created by alv18 on 03/05/2025.
//

#ifndef OBJECT3D_H
#define OBJECT3D_H
#include <string>

#include <glad/glad.h>
#include "glm.hpp"
#include "gtc/type_ptr.hpp"
#include "Shader.h"
#include "Camera3D.h"
#include "Mesh.h"

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
    // int loadTextureFromFile(char const* filename);
    void setTextureId(int textureId) {mTexture = textureId;}

    virtual void render(double deltaTime, Camera3D *camera);

    virtual void update(double deltaTime); // The goal is not to use it (very much)
    virtual void setPosition(glm::vec3 position);
    void setRotation(glm::vec3 rotationAxis, float angle);
    void setScale(glm::vec3 scale);
    glm::vec3 getPosition() {return mPosition;}
    virtual void initShader(std::string shaderName);
    Mesh* mMesh;

protected:
    std::string mShaderName;
    GLuint mTexture;
    // int mTextureIndex;
    // bool mIsTransparent;

    Shader* mShader;

    GLint mUniformModel;
    GLint mUniformProjection;
    GLint mUniformView;

    glm::vec3 mPosition;
    glm::quat mRotation;
    // glm::vec3 mRotationAxis;
    // float mRotationAngle;
    glm::vec3 mScale;

};



#endif //OBJECT3D_H
