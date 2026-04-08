//
// Created by alv18 on 04/05/2025.
//

#include "TexturedLitPlane.h"

#include <iostream>
#include <ostream>

#include "stb_image.h"

void TexturedLitPlane::render(double time, double deltaTime, Camera3D *camera) {
    mShader->Use();
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Lighting need to know camera position:
    glm::vec3 viewPosition = camera->getPosition();
    glUniform3f(mViewPositionUniform, viewPosition.x, viewPosition.y, viewPosition.z);

    // CHANGE RENDER TRIANGLES NUMBER TO 6

    Object3D::render(time, deltaTime, camera);
}
// TODO: debugear y refactorizar las uniforms
void TexturedLitPlane::initKnownShader() {
    Object3D::initShader(mShaderName);
    // Assign extra uniforms: the light color, the light position. The texture should be managed from main loop
    mShader->Use();

    mLightColorUniform = glGetUniformLocation(mShader->getID(), "lightColor");
    glUniform3f(mLightColorUniform, mLightColor.x, mLightColor.y, mLightColor.z);

    mLightPositionUniform = glGetUniformLocation(mShader->getID(), "lightPosition");
    glUniform3f(mLightPositionUniform, mLightPosition.x, mLightPosition.y, mLightPosition.z);

    mViewPositionUniform = glGetUniformLocation(mShader->getID(), "viewPosition");

    if (mLightColorUniform == -1) {
        std::cerr << "lightColor uniform not found in " << mShader->mName << std::endl;
    }
    if (mLightPositionUniform == -1) {
        std::cerr << "lightPosition uniform not found in " << mShader->mName << std::endl;
    }
    if (mViewPositionUniform == -1) {
        std::cerr << "viewPosition uniform not found in " << mShader->mName << std::endl;
    }
    // Sera actualizada en render a la camera position

    /* Some extra debugging
    GLfloat lightColorCheck[3];
    glGetUniformfv(mShader->getID(), mLightColorUniform, lightColorCheck);
    printf("Uniform lightColor en GPU: (%f, %f, %f)\n", lightColorCheck[0], lightColorCheck[1], lightColorCheck[2]);
    */
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "Error on textured lit plane init shader: " << error << std::endl;
    }
}

void TexturedLitPlane::generateCube(float size) {
    mMesh = new Mesh();
    mMesh->generatePlane(size);
}

void TexturedLitPlane::prepareVAO() {


}

void TexturedLitPlane::setObjectColor(glm::vec3 color) {
  // Leave this here in case we want to do later an "add Color to texture" option
    printf("Lit texture with color still not implemented");

    /*
    mObjectColor = color;
    mShader->Use();
    glUniform3f(mObjectColorUniform, color.x, color.y, color.z);
    */
}

void TexturedLitPlane::setIlumination(glm::vec3 lightColor, glm::vec3 lightPosition) {
    mLightColor = lightColor;
    mShader->Use();
    glUniform3f(mLightColorUniform, lightColor.x, lightColor.y, lightColor.z);
    glUniform3f(mLightPositionUniform, lightPosition.x, lightPosition.y, lightPosition.z);
}

void TexturedLitPlane::updateLightPositions(glm::vec3 lightPosition) {
    mShader->Use();
    glUniform3f(mLightPositionUniform, lightPosition.x, lightPosition.y, lightPosition.z);
}

int TexturedLitPlane::loadSmokeTexture(char const *filename) {
    mShader->Use();
    glGenTextures(1, &mTexture);

    // Bind texture with index 3
    glActiveTexture(GL_TEXTURE3);
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


