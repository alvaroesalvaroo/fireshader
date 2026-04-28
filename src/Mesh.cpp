//
// Created by alvaro on 08/04/2026.
//

#include "Mesh.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string_view>
#include "glm.hpp"
#include "detail/type_vec.hpp"
#include "gtc/quaternion.hpp"

Mesh::Mesh() {
    mVAO = -1;
    mVBO = -1;
    mEBO = -1;
    mArraysToDraw = 36; // 36 in cubes, other geometrys should assign number of elements to draw

    mUseEBO = false;
    mVertexData.clear();
    mIndexData.clear();

}

Mesh::~Mesh() = default;

void Mesh::draw() {
    glBindVertexArray(mVAO);

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
}

void Mesh::clear() {
    glDeleteVertexArrays(sizeof(mVAO), &mVAO);
    glDeleteBuffers(sizeof(mVBO), &mVBO);
    glDeleteBuffers(sizeof(mEBO), &mEBO);
}

void Mesh::createCubeWithNormalsAndUV(float size) {
    mUseEBO = false;
    int dataSize = 288; // 8 * 6 * 6
    float vertices[] = {
            // positions          // normals           // texture coords
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
             0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
             0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
             0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };

    mVertexData.clear();
    mIndexData.clear();
    for (int i = 0; i < dataSize; i++) {
        mVertexData.push_back(vertices[i] * size);
    }
    prepareVAO(VertexLayout::PosNormalUV);
}
void Mesh::createCubeWithNormals(float size) {
    mUseEBO = false;
        // LA DATA ES DIFERENTE PORQUE AHORA TIENE NORMALS
    // No tenemos UV, por lo que el stride es de 6
    int dataSize = 216; // 6 * 6 * 6
    float vertices[dataSize] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    // ESTA MAL->NO USAR EBO
    // GLuint indexData[36] =
    // {
    //     // Cara frontal
    //     0, 1, 2,  2, 3, 0,
    //     // Cara trasera
    //     4, 5, 6,  6, 7, 4,
    //     // Cara izquierda
    //     0, 3, 7,  7, 4, 0,
    //     // Cara derecha
    //     1, 2, 6,  6, 5, 1,
    //     // Cara superior
    //     3, 2, 6,  6, 7, 3,
    //     // Cara inferior
    //     0, 1, 5,  5, 4, 0
    // };

    mVertexData.clear();
    mIndexData.clear();
    for (int i = 0; i < dataSize; i++) {
        mVertexData.push_back(vertices[i] * size);
    }
    // for (unsigned int i : indexData) {
    //     mIndexData.push_back(i);
    // }
    prepareVAO(VertexLayout::PosNormal);
}
void Mesh::createCubeMeshWithNoEBO(float size) {
    mUseEBO = false;
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
    // GLuint indexData[36] =
    // {
    //     // Cara frontal
    //     0, 1, 2,  2, 3, 0,
    //     // Cara trasera
    //     4, 5, 6,  6, 7, 4,
    //     // Cara izquierda
    //     0, 3, 7,  7, 4, 0,
    //     // Cara derecha
    //     1, 2, 6,  6, 5, 1,
    //     // Cara superior
    //     3, 2, 6,  6, 7, 3,
    //     // Cara inferior
    //     0, 1, 5,  5, 4, 0
    // };

    mVertexData.clear();
    mIndexData.clear();
    for (int i = 0; i < dataSize; i++) {
        mVertexData.push_back(vertexData[i] * size);
    }
    // for (unsigned int i : indexData) {
    //     mIndexData.push_back(i);
    // }
    prepareVAO(VertexLayout::PosUV);
}

void Mesh::createCubeMeshWithEBO(float size) {

    // Generate vertex and uv data

    // VBO data
    int dataSize = 120; // I would never have thought a cube was made out of no many numbers
    mUseEBO = true; // Some problem with uvs, i will still work on it if needed
    float vertexData[dataSize] = {
        // x, y, z,              u, v
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,   // 0
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,   // 1
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,   // 2
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,   // 3

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    };
    if (vertexData[0] == 0) std::cout << "Probabily bad input in TUC line 49 " << std::endl;

    //EBO data
    GLuint indexData[36] =
    {
        // Cara frontal
        0, 1, 2,  2, 3, 0,
        // Cara trasera
        4, 5, 6,  6, 7, 4,
        // Cara derecha
        1, 2, 6,  6, 5, 1,
        // Cara izquierda
        0, 3, 7,  7, 4, 0,
        // Cara superior
        3, 2, 6,  6, 7, 3,
        // Cara inferior
        0, 1, 5,  5, 4, 0
    };
    mArraysToDraw = dataSize / 6;

    mVertexData.clear();
    mIndexData.clear();
    for (int i = 0; i < dataSize; i++) {
        mVertexData.push_back(vertexData[i] * size);
    }
    for (unsigned int i : indexData) {
        mIndexData.push_back(i);
    }

    prepareVAO(VertexLayout::PosUV);
}

void Mesh::generateTrapezium(float upSide, float downSide, float height) {
    mArraysToDraw = 6;
    mUseEBO = true;
    // El stride será de 8

    const int dataSize = 8 * 4 ; // Solo una cara
    float vertices[dataSize] = {
        // x,               y,            z,      Normals            u,     v,
        -downSide/2, -height/2, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f,  0.0f,
         downSide/2, -height/2, 0.0f,  0.0f, 0.0f, 1.0f,  1.0f,  0.0f,
         upSide/2,    height/2, 0.0f,  0.0f, 0.0f, 1.0f,  1.0f,  1.0f,
        -upSide/2,    height/2, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f,  1.0f,
    };

    GLuint indexData[6] =
    {
        // Cara frontal
        0, 1, 2,  2, 3, 0
    };

    mVertexData.clear();
    mIndexData.clear();
    for (int i = 0; i < dataSize; i++) {
        if (i % 8 < 3) { // Solo modificar x, y, z
            mVertexData.push_back(vertices[i]);
        } else {
            mVertexData.push_back(vertices[i]); // Mantener normales y UVs sin cambios
        }
    }
    for (unsigned int i : indexData) {
        mIndexData.push_back(i);
    }
    prepareVAO(VertexLayout::PosNormalUV);
}
void Mesh::generatePlane(float size) {

    mArraysToDraw = 6;
    mUseEBO = true;
    // El stride será de 8

    const int dataSize = 8 * 4 ; // Solo una cara
    float vertices[dataSize] = {
        // x, y, z,         // Normals            // u, v,
        -0.5f, -0.5f, 0.f,  0.0f,  0.0f, 1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, 0.f,  0.0f,  0.0f, 1.0f, 1.0f, 0.0f,
        0.5f,  0.5f, 0.f,  0.0f,  0.0f, 1.0f, 1.0f, 1.0f,
        -0.5f,  0.5f, 0.f,  0.0f,  0.0f, 1.0f, 0.0f, 1.0f
    };

    GLuint indexData[6] =
    {
        // Cara frontal
        0, 1, 2,  2, 3, 0
    };

    mVertexData.clear();
    mIndexData.clear();
    for (int i = 0; i < dataSize; i++) {
        if (i % 8 < 3) { // Solo modificar x, y, z
            mVertexData.push_back(vertices[i] * size);
        } else {
            mVertexData.push_back(vertices[i]); // Mantener normales y UVs sin cambios
        }
    }
    for (unsigned int i : indexData) {
        mIndexData.push_back(i);
    }
    prepareVAO(VertexLayout::PosNormalUV);
}

void Mesh::loadMeshFromFile(const char *filename) {
    mUseEBO = true;
    // For now, only OBJ files
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open mesh file " << filename << std::endl;
    }

    std::string line;
    int lineNumber = 0;

    // Data of normal and uv has to be expanded to have as many as vertex
    std::vector<GLfloat> shortNormalData;
    std::vector<GLfloat> normalData;
    std::vector<GLfloat> shortUVData;
    std::vector<GLfloat> UVData;
    bool vertexDataFinished = false;

    while (std::getline(file, line)) {
        std::stringstream lineStream(line);
        std::string identifier;
        lineStream >> identifier;
        lineNumber++;

        // Fill vertex data

        if (identifier == "v") {  // Línea de vértice

            GLfloat x, y, z;
            lineStream >> x >> y >> z;
            mVertexData.push_back(x);
            mVertexData.push_back(y);
            mVertexData.push_back(z);
        }
        // Fill uv data. In Blender uvdata /2 should match vertexdata / 3
        else if (identifier == "vt") {

            GLfloat u, v;
            lineStream >> u >> v;
            shortUVData.push_back(u);
            shortUVData.push_back(v);
        }
        else if (identifier == "vn") {

            GLfloat nx, ny, nz;
            lineStream >> nx >> ny >> nz;
            shortNormalData.push_back(nx);
            shortNormalData.push_back(ny);
            shortNormalData.push_back(nz);
        }
        else if (identifier == "f") {
            if (!vertexDataFinished) {
                vertexDataFinished = true;
                normalData.resize(mVertexData.size()); // Assign the same size as the vertex data
                UVData.resize(mVertexData.size() * 2 / 3);
            }


            for (int i = 0; i < 3; i++) {  // Procesar cada vértice de la cara con formato f p/u/n p/u/n p/u/n

                int vertexIndex, uvIndex, normalIndex;
                char slash;

                lineStream >> vertexIndex >> slash >> uvIndex >> slash >> normalIndex;

                if (vertexIndex == 0) {
                    std::cerr << "Face referenced to vertex number 0. This should not happen in a obj file" << std::endl;
                    return;
                }
                mIndexData.push_back(vertexIndex - 1);

                if ((uvIndex == 0)) {
                    std::cerr << "Face referenced to UV number 0. This should not happen in a obj file" << std::endl;
                    return;
                }

                // Ad two numbers to UVData in the position vertex index suggests
                UVData.at((vertexIndex-1) * 2 + 0) = shortUVData.at((uvIndex-1) * 2 + 0);
                UVData.at((vertexIndex-1) * 2 + 1) = shortUVData.at((uvIndex-1) * 2 + 1);

                if (normalIndex == 0) {
                    std::cerr << "Face referenced to Normal number 0. This should not happen in a obj file" << std::endl;
                    return;
                }
                // Ad three numbers to NormalData in the position vertex index suggests
                normalData.at((vertexIndex-1) * 3 + 0) = shortNormalData.at((normalIndex-1) * 3 + 0);
                normalData.at((vertexIndex-1) * 3 + 1) = shortNormalData.at((normalIndex-1) * 3 + 1);
                normalData.at((vertexIndex-1) * 3 + 2) = shortNormalData.at((normalIndex-1) * 3 + 2);
            }

        }
    }

    file.close();

    // NOW, join VertexData, NormalData and UVData
    // Para hacerlo más efectivo, y compatible con la funcion prepareVAO

    size_t vertexCount = mVertexData.size() / 3;
    std::vector<GLfloat> allData;
    allData.reserve(vertexCount * 8); // Stride 8: assign now all space
    for (int i = 0; i < vertexCount; i++) {
        allData.push_back(mVertexData[i * 3 + 0]);
        allData.push_back(mVertexData[i * 3 + 1]);
        allData.push_back(mVertexData[i * 3 + 2]);
        allData.push_back(normalData[i * 3 + 0]);
        allData.push_back(normalData[i * 3 + 1]);
        allData.push_back(normalData[i * 3 + 2]);
        allData.push_back(UVData[i * 2 + 0]);
        allData.push_back(UVData[i * 2 + 1]);
    }

    mVertexData = std::move(allData); // Transfer all data to vertexdata
    normalData.clear();
    UVData.clear();
    std::cout << "Loaded mesh " << filename << " with " << vertexCount << " vertices and " << mVertexData.size() << " data size" << std::endl;

    prepareVAO(VertexLayout::PosNormalUV);
}

void Mesh::prepareVAO(VertexLayout layout) {
    std::cout << "Preparing VAO with useEBO: "<< mUseEBO << " and layout type: " << (int) layout <<std::endl;

    // Create VAO
    glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO);

    //Create VBO
    glGenBuffers(1, &mVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferData(GL_ARRAY_BUFFER, mVertexData.size() * sizeof(GLfloat), mVertexData.data(), GL_STATIC_DRAW);
    GLenum error;
    if ((error = glGetError()) != GL_NO_ERROR) {
        std::cerr << "ERROR buffering data: " << error << std::endl;
    }
    // Ensure integrity of Mesh:
    size_t totalFloats = mVertexData.size();
    int expectedStride = 0;
    if (layout == VertexLayout::PosUV) expectedStride = 5;
    else if (layout == VertexLayout::PosNormal) expectedStride = 6;
    else if (layout == VertexLayout::PosUVColor || layout == VertexLayout::PosNormalUV) expectedStride = 8;
    if (totalFloats % expectedStride != 0) {
        std::cerr << "ERROR DE INTEGRIDAD EN MESH" << std::endl;
        std::cerr << "Total floats: " << totalFloats << " no es divisible por stride: " << expectedStride << std::endl;
        std::cerr << "Vértices sobrantes/faltantes: " << (totalFloats % expectedStride) << std::endl;
        // No detenemos el programa, pero ya sabemos que se verá mal
    } else {
        std::cout << "Mesh Integrity OK: " << (totalFloats / expectedStride) << " vertices detectados." << std::endl;
    }
    //////////////////////////////////////////////////////////////////////
    if (layout == VertexLayout::PosUV) {
        // Stride 5
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);
    }
    else if (layout == VertexLayout::PosUVColor) {
        int stride = 8 * sizeof(GLfloat);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(GLfloat)));
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(5 * sizeof(GLfloat)));
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
    }
    else if (layout == VertexLayout::PosNormal) {
        // Stride 6: 3 pos + 3 normal
        GLsizei stride = 6 * sizeof(GLfloat);

        // Posición (Location 0)
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
        glEnableVertexAttribArray(0);

        // Normal (Location 1)
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);
    }
    else if (layout == VertexLayout::PosNormalUV) {
        int stride = 8 * sizeof(float);
        // Atributo 0: Posición (x, y, z) - Offset 0
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
        // Atributo 1: Normales (nx, ny, nz) - Offset 3
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
        // Atributo 2: UVs (u, v) - Offset 6
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
    }

    //Create IBO
    if (mUseEBO) {
        if (mIndexData.empty()) {
            std::cerr << "EBO not initialized but set to true " << std::endl;
        }
        glGenBuffers(1, &mEBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndexData.size() * sizeof (GLuint), mIndexData.data(), GL_STATIC_DRAW);
    }
     glBindVertexArray(0);

    if ((error = glGetError()) != GL_NO_ERROR) {
        std::cerr << "ERROR transfering mesh data: " << error << std::endl;
    }

}