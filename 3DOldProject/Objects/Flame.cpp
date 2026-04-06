//
// Created by alv18 on 06/05/2025.
//

#include "Flame.h"
#include <fstream>
#include <sstream>

void Flame::loadFlamesData(char const* filename, int numberOfShapes) {
    mNumberOfShapes = numberOfShapes;
    for (int i = 0; i < numberOfShapes; i++) {

        std::string fullName = std::string(filename) + std::to_string(i+1) + ".obj";
        loadSingleFlameData(fullName, i);
    }
    // TODO: Debug for using geometry with "normal" shaders:
    mVertexData = mVertexLists[0];

    printf("Some vertex elements: %f, %f, %f, %f, %f, %f\n", mVertexData[0], mVertexData[1], mVertexData[2], mVertexData[3],mVertexData[4], mVertexData[5]);
    //printf("Some index elements: %d, %d, %d, %d, %d, %d\n", mIndexData[0], mIndexData[2], mIndexData[3], mIndexData[4], mIndexData[5]);
    printf("Loaded %d flame shapes with vertex data sizes %d, %d, %d\n", numberOfShapes, mVertexLists[0].size()/3, mVertexLists[1].size()/3, mVertexLists[2].size()/3);
    printf("    uv data size %d, indexes size %d\n", mUVData.size()/2, mIndexData.size());
}

void Flame::loadSingleFlameData(std::string filename, int listIndex) {

    std::ifstream file(filename);
    if (!file.is_open()) {
        printf("Failed to open mesh file %s\n", filename.c_str());
    }

    std::string line;
    int lineNumber = 0;

    // Data of normal and uv has to be expanded to have as many as vertex
    std::vector<GLfloat> shortNormalData;
    std::vector<GLfloat> shortUVData;
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
            mVertexLists[listIndex].push_back(x);
            mVertexLists[listIndex].push_back(y);
            mVertexLists[listIndex].push_back(z);
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
                mUVData.resize(mVertexLists[listIndex].size() * 2 / 3);
            }

            for (int i = 0; i < 3; i++) {  // Procesar cada vértice de la cara con formato f p/u/n p/u/n p/u/n

                int vertexIndex, uvIndex, normalIndex;
                char slash;

                lineStream >> vertexIndex >> slash >> uvIndex >> slash >> normalIndex;

                if (vertexIndex == 0) {
                    printf("Face referenced to vertex number 0. This should not happen in a obj file\n");
                    return;
                }
                if ((uvIndex == 0)) {
                    printf("Face referenced to uv number 0. This should not happen in a obj file\n");
                    return;
                }
                // ONLY PUSH INDEX DATA IN FIRST SHAPE
                if (listIndex == 0) {
                    mIndexData.push_back(vertexIndex - 1);
                }
                mUVData.at((vertexIndex-1) * 2 + 0) = shortUVData.at((uvIndex-1) * 2 + 0);
                mUVData.at((vertexIndex-1) * 2 + 1) = shortUVData.at((uvIndex-1) * 2 + 1);

            }
        }
    }

    file.close();

}

void Flame::prepareGeometryForShader() {
    mUseEBO = true;


    glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO);
    /*
    // Crear VBOs diferentes para cada lista de vertices
    for (int shapeIndex = 0; shapeIndex < mNumberOfShapes; shapeIndex++) {
        glGenBuffers(1, &mVBOList[shapeIndex]);
        glBindBuffer(GL_ARRAY_BUFFER, mVBOList[shapeIndex]);
        glBufferData(GL_ARRAY_BUFFER, mVertexLists[shapeIndex].size() * sizeof(GLfloat), mVertexLists[shapeIndex].data(), GL_STATIC_DRAW);
    }




    // Asignar a cada de las listas de vertex uno de los parametros de entrada (layout = 0, 1, 2, ...)
    int stride = 0;   // La data esta en listas separadas, y por tanto estan contiguos
    for (int shapeIndex = 0; shapeIndex < mNumberOfShapes; shapeIndex++) {
        glBindBuffer(GL_ARRAY_BUFFER, mVBOList[shapeIndex]);
        glVertexAttribPointer(shapeIndex, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*)0);
        glEnableVertexAttribArray(shapeIndex);
    }*/

    // TODO THIS DEBUG
    glGenBuffers(1, &mVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferData(GL_ARRAY_BUFFER, mVertexData.size() * sizeof(GLfloat), mVertexData.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
    glEnableVertexAttribArray(0);


    // Crear VBO para las UVs
    glGenBuffers(1, &mVBO_UV);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO_UV);
    glBufferData(GL_ARRAY_BUFFER, mUVData.size() * sizeof(GLfloat), mUVData.data(), GL_STATIC_DRAW);

    // LAYOUT POSITION mNumberOfShapes para los UVs (van despues de los shapes)
    glVertexAttribPointer(mNumberOfShapes, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
    glEnableVertexAttribArray(mNumberOfShapes);

    // Crear VBO para los colors
    // TODO: PERSONALIZAR COLORS
    mColorData.resize(mVertexLists[0].size());
    std::fill(mColorData.begin(), mColorData.end(), 1.0f);

    glGenBuffers(1, &mVBO_Color);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO_Color);
    glBufferData(GL_ARRAY_BUFFER, mColorData.size() * sizeof(GLfloat), mColorData.data(), GL_STATIC_DRAW);

    // Layout POSITION mNumberOfShapes + 1
    glVertexAttribPointer(mNumberOfShapes + 1, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
    glEnableVertexAttribArray(mNumberOfShapes + 1);

    // The one and only IBO
    glGenBuffers(1, &mEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndexData.size() * sizeof (GLuint), mIndexData.data(), GL_STATIC_DRAW);

    // Clean
    glBindVertexArray(0);
}

void Flame::initShader(const char* filename) {
    Object3D::initShader(filename); // Inits Model, Projection and View transforms
    // Assign other uniforms
    int shaderID = mShader->getID();
    mTimeUniform = glGetUniformLocation(shaderID, "time");
}

void Flame::render(double time, double deltaTime, Camera3D* camera) {
    if (!mUseEBO) printf("For some reason flame bool mUseEbo set to false\n");
    Object3D::render(time, deltaTime, camera);
}