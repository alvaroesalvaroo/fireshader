//
// Created by alv18 on 04/05/2025.
//

#include "NormalMapMesh.h"

#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "../stb_image.h"



void NormalMapMesh::render(double deltaTime, Camera3D *camera){
  // Before rendering, we must assign normal texture
  mShader->Use();
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, mNormalMapTexture);

  TexturedLitMesh::render(deltaTime, camera);

}


int NormalMapMesh::loadNormalTexture(char const* filename){
  // TODO: HACER LAS MATES DE UNA NORMAL TEXTURE (sale mal)
  mShader->Use();
  glGenTextures(1, &mNormalMapTexture);

  // Bind texture with index 1
  // Nombre que aparece en el shader
  glUniform1i(glGetUniformLocation(mShader->getID(), "normalTexture"), 1); // GL_TEXTURE0
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, mNormalMapTexture);
  // Load file
  int width, height, nrChannels;
  stbi_set_flip_vertically_on_load(true);
  unsigned char *pixels = stbi_load(filename, &width, &height, &nrChannels, 0);
  if (!pixels) {
    printf("Failed to load normal texture: %s\n", stbi_failure_reason());
    return -1;
  }

  printf("Normal map %s loaded with nChannels: %d, textureID %d\n", filename, nrChannels, mNormalMapTexture);

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
  printf("mNormalMapTexture: %d\n", mNormalMapTexture);
  return mNormalMapTexture;
}



