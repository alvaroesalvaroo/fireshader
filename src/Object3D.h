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
#include "Material.h"
#include "Mesh.h"


class Object3D {
public:
    Object3D();
    virtual ~Object3D();
    // int loadTextureFromFile(char const* filename);
    void setTextureId(int textureId) {mTexture = textureId;}
    void setSecondaryTextureId(int textureId2) {mTexture2 = textureId2;}

    virtual void render(float dt, Camera3D *camera);

    virtual void update(float dt); // The goal is not to use it (very much)
    virtual void setPosition(glm::vec3 position);

    void setPosition(float px, float py, float pz);

    void setRotation(glm::vec3 rotationAxis, float angle);
    void setScale(glm::vec3 scale);

    void setScale(float sx, float sy, float sz);

    glm::vec3 getPosition() {return mPosition;}
    void setShader(Shader *shader);
    void setMesh(Mesh *mesh) {mMesh = mesh;}
    void setMaterial(Material *mat) {mMaterial = mat;};
protected:
    Mesh* mMesh;
    Material* mMaterial;

    std::string mShaderName;
    GLint mTexture;
    GLint mTexture2;
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
