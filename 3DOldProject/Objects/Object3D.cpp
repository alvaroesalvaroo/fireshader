//
// Created by alv18 on 03/05/2025.
//

#include "Object3D.h"



#include <vector>
#include <string>

#include <glad/glad.h>
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "../glm/glm/gtx/string_cast.hpp"

#include "../Shader.h"
#include "../../src/stb_image.h"

Object3D::Object3D() {
    // Initialize an object in coordinates origin with no shader, vao, vbo, ebo or texture
    mPosition = glm::vec3(0.0f);
    mScale = glm::vec3(1.0f);
    mShader = nullptr;
    mVAO = -1;
    mVBO = -1;
    mEBO = -1;
    mArraysToDraw = 36;   // 36 in cubes, other geometrys should assign number of elements to draw
    mTexture = -1;
    mUniformModel = -1;
    mVertexData = std::vector<GLfloat>();
    mIndexData = std::vector<GLuint>();

}
Object3D::~Object3D()
{
    clearGPU();
    mVertexData.clear();
    mIndexData.clear();
}

int Object3D::loadTextureFromFile(char const *filename, int textureIndex) {

    GLenum TEXTURE0 = GL_TEXTURE0;
    mShader->Use();
    glGenTextures(1, &mTexture);

    // Bind texture with index 0
    glActiveTexture(TEXTURE0 + textureIndex);
    glBindTexture(GL_TEXTURE_2D, mTexture);
    // Load file
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *pixels = stbi_load(filename, &width, &height, &nrChannels, 0);
    if (!pixels) {
        printf("Failed to load texture: %s\n", stbi_failure_reason());
        return -1;
    }

    printf("Texture %s loaded with nChannels: %d, textureID %d\n", filename, nrChannels, mTexture);

    GLint Mode = 0;
    if (nrChannels == 3) {
        Mode = GL_RGB;
    }
    else if (nrChannels == 4) {
        Mode = GL_RGBA;
        mIsTransparent = true;
    }
        //type, mipmaplevel, internalformat, width, height, border (always 0), format, type, pixelsdata
    glTexImage2D(GL_TEXTURE_2D, 0, Mode, width, height, 0, Mode, GL_UNSIGNED_BYTE, pixels);
    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        printf("ERROR on texture: %d\n", err);
    }
    glGenerateMipmap(GL_TEXTURE_2D);


    // Wrapping mode in both direction
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Scaling modes
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Cleaning
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(pixels);

    return mTexture;
}

void Object3D::generateCube(float size) {

    // VBO data
    int dataSize = 180; // I would never have thought a cube was made out of 180 numbers
    float vertexData[dataSize] = {
        // x, y, z,              u, v
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,   // 0
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,   // 1
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,   // 2
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    //EBO data
    GLuint indexData[36] =
    {
        // Cara frontal
        0, 1, 2,  2, 3, 0,
        // Cara trasera
        4, 5, 6,  6, 7, 4,
        // Cara izquierda
        0, 3, 7,  7, 4, 0,
        // Cara derecha
        1, 2, 6,  6, 5, 1,
        // Cara superior
        3, 2, 6,  6, 7, 3,
        // Cara inferior
        0, 1, 5,  5, 4, 0
    };

    mVertexData.clear();
    mIndexData.clear();
    for (int i = 0; i < dataSize; i++) {
        mVertexData.push_back(vertexData[i] * size);
    }
    for (unsigned int i : indexData) {
        mIndexData.push_back(i);
    }
    printf("Generated generic cube with arist %1.1f\n ", size);
    prepareVAO();

}

// Classical transform update. Not doing anything since the goal is to become a shader master
void Object3D::update(double time, double deltaTime) {

}

// The idea is not to use this method since each class will have its own associated shader
// Shader should be created on each class initialization
void Object3D::setShader(Shader *shader) {
    mShader = shader;
    mShader->Use();
    mUniformModel = glGetUniformLocation(mShader->getID(), "model");
    mUniformProjection = glGetUniformLocation(mShader->getID(), "projection");
    mUniformView = glGetUniformLocation(mShader->getID(), "view");
    printf("Shader externally setted and assigned model, projection and view uniforms\n");
}

void Object3D::initShader(char const *shaderName) {
    mShader = new Shader();
    mShader->init(shaderName);

    mUniformModel = glGetUniformLocation(mShader->getID(), "model");
    mUniformProjection = glGetUniformLocation(mShader->getID(), "projection");
    mUniformView = glGetUniformLocation(mShader->getID(), "view");

    printf("Uniforms model, projection and view assigned in shader %s\n", shaderName);
}

Shader *Object3D::getShader() {

    if (mShader == NULL) {
        printf("Warning. Object was asked for not initialized shader\n");
    }
    return mShader;
}



void Object3D::render(double time, double deltaTime, Camera3D *camera) {

    // Generic render
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);   // Handly debug

    mShader->Use();
    camera->updateProjectionUniform(mUniformProjection);
    camera->updateViewUniform(mUniformView);
    glActiveTexture(GL_TEXTURE0 + mTextureIndex);
    glBindTexture(GL_TEXTURE_2D, mTexture);

    // Set Geometry
    glBindVertexArray(mVAO);

    //Do matrix model movements
    glm::mat4 transformMatrix = glm::mat4();
    transformMatrix = glm::translate(transformMatrix, mPosition);
    // We need to initiate angle and axis or something:
    // transformMatrix = glm::rotate(transformMatrix, mRotationAngle, mRotationAxis);
    transformMatrix = glm::scale(transformMatrix, mScale);

    glUniformMatrix4fv(mUniformModel, 1, GL_FALSE, glm::value_ptr(transformMatrix));

    if (mUseEBO) {
        // Draw Elements with EBO
        glDrawElements(GL_TRIANGLES, mIndexData.size() , GL_UNSIGNED_INT, nullptr);
    }
    else {
        //Draw VAO directly, with no EBO
        glDrawArrays(GL_TRIANGLES, 0, mArraysToDraw);
    }


    // Cleaning
    glBindVertexArray(0);
    glUseProgram(NULL);
}

void Object3D::prepareVAO() {
    printf("Called Object3D::prepareVAO. Specific method should be called\n");
    // Create VAO
    glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO);

    //Create VBO
    glGenBuffers(1, &mVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferData(GL_ARRAY_BUFFER, mVertexData.size() * sizeof(GLfloat), mVertexData.data(), GL_STATIC_DRAW);
    GLenum error;
    if ((error = glGetError()) != GL_NO_ERROR) {
        printf("ERROR buferring data: %d\n", error);
    }
    //////////////////////////////////////////////////////////////////////
    // Assuming our shader will have 2 attributes pointer:
    // layout (location = 0) in vec3 LVertexPos; -> (location=0, size = 3, offset =0)
    // layout (location = 1) in vec2 texCoor;   ->  (location = 1, size = 2, offset = 3)
    // with a stride of 5 floats

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));

    glEnableVertexAttribArray(0); // position
    glEnableVertexAttribArray(1); // Texture coor
    glEnableVertexAttribArray(2); // Color Vertex

    //Create IBO
    glGenBuffers(1, &mEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndexData.size() * sizeof (GLuint), mIndexData.data(), GL_STATIC_DRAW);
    glBindVertexArray(0);

}

void Object3D::clearGPU() {
    // Only should be called on destroy
    glDeleteVertexArrays(sizeof(mVAO), &mVAO);
    glDeleteBuffers(sizeof(mVBO), &mVBO);
    glDeleteBuffers(sizeof(mEBO), &mEBO);
}

void Object3D::setPosition(glm::vec3 position) {
    mPosition = position;
}
void Object3D::setRotation(glm::vec3 rotationAxis, float angle) {
    mRotationAxis = rotationAxis;
}
void Object3D::setScale(glm::vec3 scale) {
    mScale = scale;
}



