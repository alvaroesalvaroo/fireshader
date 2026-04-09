//
// Created by alv18 on 03/05/2025.
//

#include "Object3D.h"

#include <iostream>
#include <vector>
#include <string>

#include <glad/glad.h>
#include "glm.hpp"
#include "ResourceManager.h"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "../glm/glm/gtx/string_cast.hpp"

#include "Shader.h"
#include "stb_image.h"

Object3D::Object3D() {
    // Initialize an object in coordinates origin with no shader, vao, vbo, ebo or texture
    mPosition = glm::vec3(0.0f);
    mRotation = glm::quat();
    mScale = glm::vec3(1.0f);
    mShader = nullptr;
    mTexture = -1;
    mUniformModel = -1;
    mMesh = nullptr;
}
Object3D::~Object3D()
{
    // If mesh is created here: delete mMesh;
    mMesh = nullptr;
}
//
// int Object3D::loadTextureFromFile(char const *filename) {
//
//     std::cerr << "Call to legacy Object3D::loadTextureFromFile. Call aborted" << std::endl;
//     return;
//     GLenum TEXTURE0 = GL_TEXTURE0;
//     mShader->Use();
//     glGenTextures(1, &mTexture);
//
//     // Bind texture with index 0
//     glActiveTexture(TEXTURE0);
//     glBindTexture(GL_TEXTURE_2D, mTexture);
//
//     FILE* f = fopen(filename, "rb");
//     if(!f) { std::cerr << "File " << filename << " not found "<<std::endl; }
//     else { fclose(f); }
//
//     // Load file
//     int width, height, nrChannels;
//     stbi_set_flip_vertically_on_load(true);
//     unsigned char *pixels = stbi_load(filename, &width, &height, &nrChannels, 0);
//
//     if (!pixels) {
//         std::cerr << "stbi error loading " << filename << "  reason is " << stbi_failure_reason() <<std::endl;
//         // printf("Failed to load texture: %s\n", stbi_failure_reason());
//         return -1;
//     }
//
//     GLenum err = glGetError();
//     if (err != GL_NO_ERROR) {
//         std::cerr << "GLError loading " << filename << ": " << err << std::endl;
//     }
//
//
//
//     printf("Texture %s loaded with nChannels: %d, textureID %d\n", filename, nrChannels, mTexture);
//
//     GLint Mode = 0;
//     if (nrChannels == 3) {
//         Mode = GL_RGB;
//     }
//     else if (nrChannels == 4) {
//         Mode = GL_RGBA;
//         // mIsTransparent = true;
//     }
//         //type, mipmaplevel, internalformat, width, height, border (always 0), format, type, pixelsdata
//     glTexImage2D(GL_TEXTURE_2D, 0, Mode, width, height, 0, Mode, GL_UNSIGNED_BYTE, pixels);
//     err = glGetError();
//     if (err != GL_NO_ERROR) {
//         std::cerr << "Error in glTexImage2D with filename " << filename << "  reason is "<< stbi_failure_reason() <<std::endl;
//     }
//     glGenerateMipmap(GL_TEXTURE_2D);
//
//
//     // Wrapping mode in both direction
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//     // Scaling modes
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//
//     // Cleaning
//     glBindTexture(GL_TEXTURE_2D, 0);
//     stbi_image_free(pixels);
//
//     return mTexture;
// }


// Classical transform update. Not doing anything since the goal is to become a shader master
void Object3D::update(double time, double deltaTime) {

}


// Init shader and associate model-projection-view uniforms
void Object3D::initShader(std::string shaderName) {

    this->mShader = &ResourceManager::LoadShader(shaderName);

    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        std::cerr << "Error before initing shader " << err <<std::endl;
    }

    mUniformModel = glGetUniformLocation(mShader->getID(), "model");
    mUniformProjection = glGetUniformLocation(mShader->getID(), "projection");
    mUniformView = glGetUniformLocation(mShader->getID(), "view");

    err = glGetError();
    if (err != GL_NO_ERROR) {
        std::cerr << "Error after initing shader " << err <<std::endl;
    }

    if (mUniformModel == -1 || mUniformProjection == -1 || mUniformView == -1) {
        std::cerr << "Uniforms model, projection or view NOT assigned in shader " << shaderName <<std::endl;
    }

    // std::cout << "Uniforms model, projection and view assigned in shader" << shaderName << std::endl;
}



void Object3D::render(double time, double deltaTime, Camera3D *camera) {

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);   // Handly debug

    mShader->Use();
    camera->updateProjectionUniform(mUniformProjection);
    camera->updateViewUniform(mUniformView);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mTexture);

    //Do matrix model movements
    glm::mat4 transformMatrix = glm::mat4();
    transformMatrix = glm::translate(transformMatrix, mPosition);
    // Rotation not supported now:
    // transformMatrix = glm::rotate(transformMatrix, mRotationAngle, mRotationAxis);
    transformMatrix = glm::scale(transformMatrix, mScale);
    glUniformMatrix4fv(mUniformModel, 1, GL_FALSE, glm::value_ptr(transformMatrix));

    // Render the mesh
    if (mMesh != nullptr) {
        mMesh->draw();
    }
    else {
        std::cout << "GameObject " << this << " not draw because has not mesh";
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(NULL);
}


void Object3D::setPosition(glm::vec3 position) {
    mPosition = position;
}
void Object3D::setRotation(glm::vec3 rotationAxis, float angle) {
    mRotation = glm::angleAxis(glm::radians(angle), rotationAxis);
}
void Object3D::setScale(glm::vec3 scale) {
    mScale = scale;
}



