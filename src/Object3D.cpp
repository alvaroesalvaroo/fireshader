//
// Created by alv18 on 03/05/2025.
//

#include "Object3D.h"

#include <iostream>
#include <string>

#include <glad/glad.h>
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "../glm/glm/gtx/string_cast.hpp"

#include "Shader.h"
#include "stb_image.h"

Object3D::Object3D() {
    // Initialize an object in coordinates origin with no shader, vao, vbo, ebo or texture
    mPosition = glm::vec3(0.0f);
    mRotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f); // identidad
    mScale = glm::vec3(1.0f);

    mTexture = -1;
    mTexture2 = -1;
    mUniformModel = -1;

    mShader = nullptr;
    mMesh = nullptr;
    mMaterial = nullptr;
}

Object3D::~Object3D()
{
    // No owners
}

// Classical transform update. Not doing anything since the goal is to become a shader master
void Object3D::update(float dt) {

}

// Init shader and associate model-projection-view uniforms
void Object3D::setShader(Shader *shader) {

    this->mShader = shader;

    mUniformModel = glGetUniformLocation(mShader->getID(), "model");
    mUniformProjection = glGetUniformLocation(mShader->getID(), "projection");
    mUniformView = glGetUniformLocation(mShader->getID(), "view");

    if (mUniformModel == -1 || mUniformProjection == -1 || mUniformView == -1) {
        std::cerr << "Uniforms model, projection or view NOT assigned in shader " << shader->mName <<std::endl;
    }

    // std::cout << "Uniforms model, projection and view assigned in shader" << shaderName << std::endl;
}



void Object3D::render(float dt, Camera3D *camera) {

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);   // Handly debug

    mShader->Use();
    if (mMaterial) {
        mMaterial->updateMaterialToShader(mShader);
    }
    camera->updateProjectionUniform(mUniformProjection);
    camera->updateViewUniform(mUniformView);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mTexture);
    if (mTexture2 != -1) {
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, mTexture2);
    }
    //Do matrix model movements
    glm::mat4 transformMatrix = glm::mat4(1.0f);
    transformMatrix = glm::translate(transformMatrix, mPosition);
    // Rotation with quaternion:
    transformMatrix = transformMatrix * glm::mat4_cast(mRotation);
    // Scale
    transformMatrix = glm::scale(transformMatrix, mScale);
    glUniformMatrix4fv(mUniformModel, 1, GL_FALSE, glm::value_ptr(transformMatrix));

    // Render the mesh
    if (mMesh != nullptr) {
        mMesh->draw();
    }
    else {
        std::cout << "GameObject " << this << " wont draw because has not mesh";
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(NULL);
}


void Object3D::setPosition(glm::vec3 position) {
    mPosition = position;
}
void Object3D::setPosition(float px, float py, float pz) {
    mPosition = glm::vec3(px, py, pz);
}
void Object3D::setRotation(glm::vec3 rotationAxis, float angle) {
    mRotation = glm::angleAxis(glm::radians(angle), rotationAxis);
}
void Object3D::setScale(glm::vec3 scale) {
    mScale = scale;
}
void Object3D::setScale(float sx, float sy, float sz) {
    mScale = glm::vec3(sx, sy, sz);
}
