//
// Created by alvaro on 08/04/2026.
//

#ifndef MESH_H
#define MESH_H
#include <vector>

#include <glm.hpp>
#include "detail/type_vec.hpp"
#include "glad/glad.h"

enum class VertexLayout {
    PosUV,      // 5 floats: [X, Y, Z, U, V]
    PosUVColor, // 8 floats: [X, Y, Z, U, V, R, G, B]
    PosNormal, // 6 floats: [X, Y, Z, NX, NY, NZ]
    PosNormalUV // 8 floats: [X, Y, Z, NX, NY, NZ, U, V]

};

class Mesh {
public:
    Mesh();
    ~Mesh();

    void draw();
    void clear();

    void createCubeWithNormals(float size);
    void createCubeWithNormalsAndUV(float size);
    void createCubeMeshWithNoEBO(float size);
    void createCubeMeshWithEBO(float size);

    void generateTrapezium(float upSide, float downSide, float height);

    void generatePlane(float size);

    void loadMeshFromFile(const char* filename);

private:
    void prepareVAO(VertexLayout layout);


    GLuint mVAO, mVBO, mEBO;
    std::vector<GLfloat> mVertexData;
    std::vector<GLuint> mIndexData;
    // Used only with EBO:
    std::vector<GLfloat> mNormalData;
    std::vector<GLfloat> mUVData;

    int mArraysToDraw;  // 36 for cubes, 6 for planes
    bool mUseEBO;

};


#endif //MESH_H