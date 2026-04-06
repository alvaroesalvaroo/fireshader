//
// Created by alv18 on 04/05/2025.
//

#include "TexturedLitMesh.h"

#include <fstream>
#include <sstream>
#include <vector>
#include <string>


void TexturedLitMesh::render(double time, double deltaTime, Camera3D *camera) {
    // Generic render
    mShader->Use();
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Lighting need to know camera position:
    glm::vec3 viewPosition = camera->getPosition();
    glUniform3f(mViewPositionUniform, viewPosition.x, viewPosition.y, viewPosition.z);

    // CHANGE RENDER TRIANGLES NUMBER TO 6
    Object3D::render(time, deltaTime, camera);


}


void TexturedLitMesh::loadMeshFromFile(char const *filename) {
    // For now, only OBJ files
    std::ifstream file(filename);
    if (!file.is_open()) {
        printf("Failed to open mesh file %s\n", filename);
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
                mNormalData.resize(mVertexData.size()); // Assign the same size as the vertex data
                mUVData.resize(mVertexData.size() * 2 / 3);
            }


            for (int i = 0; i < 3; i++) {  // Procesar cada vértice de la cara con formato f p/u/n p/u/n p/u/n

                int vertexIndex, uvIndex, normalIndex;
                char slash;

                lineStream >> vertexIndex >> slash >> uvIndex >> slash >> normalIndex;

                if (vertexIndex == 0) {
                    printf("Face referenced to vertex number 0. This should not happen in a obj file\n");
                    return;
                }
                mIndexData.push_back(vertexIndex - 1);

                if ((uvIndex == 0)) {
                    printf("Face referenced to uv number 0. This should not happen in a obj file\n");
                    return;
                }

                // Ad two numbers to UVData in the position vertex index suggests
                mUVData.at((vertexIndex-1) * 2 + 0) = shortUVData.at((uvIndex-1) * 2 + 0);
                mUVData.at((vertexIndex-1) * 2 + 1) = shortUVData.at((uvIndex-1) * 2 + 1);

                if (normalIndex == 0) {
                    printf("Face referenced to normal number 0. This should not happen in a obj file\n");
                    return;
                }
                // Ad three numbers to NormalData in the position vertex index suggests
                mNormalData.at((vertexIndex-1) * 3 + 0) = shortNormalData.at((normalIndex-1) * 3 + 0);
                mNormalData.at((vertexIndex-1) * 3 + 1) = shortNormalData.at((normalIndex-1) * 3 + 1);
                mNormalData.at((vertexIndex-1) * 3 + 2) = shortNormalData.at((normalIndex-1) * 3 + 2);
            }

        }
    }

    file.close();
    printf("Loaded mesh %s with %d vertices, %d UVs, %d normals and %d IndexData \n", filename, mVertexData.size()/3, mUVData.size()/2, mNormalData.size()/3, mIndexData.size());

    /* Ugly debug
    printf("First vertex: %f, %f, %f\n", mVertexData[0], mVertexData[1], mVertexData[2]);
    printf("First uv: %f, %f\n", mUVData[0], mUVData[1]);
    printf("First normal: %f, %f, %f\n", mNormalData[0], mNormalData[1], mNormalData[2]);
    printf("First triangle index: %d, %d, %d\n", mIndexData[0], mIndexData[1], mIndexData[2]);
    printf("Last triangle index: %d, %d, %d\n", mIndexData[mIndexData.size()-3], mIndexData[mIndexData.size()-2], mIndexData[mIndexData.size()-1]);
    */
    prepareMeshData();
}

void TexturedLitMesh::prepareMeshData() {

    printf("preparing VAO, 3 VBO and 3 IBO for mesh with data size: vertexData: %u, uvData: %u, normalData: %u, indexData: %u \n", mVertexData.size(), mUVData.size(), mNormalData.size(), mIndexData.size());
    mUseEBO = true;



    // Only one VAO
    glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO);

    // Crear VBOs para cada tipo de dato (Posicion, uv, normals)
    glGenBuffers(1, &mVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferData(GL_ARRAY_BUFFER, mVertexData.size() * sizeof(GLfloat), mVertexData.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &mVBO_Normal);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO_Normal);
    glBufferData(GL_ARRAY_BUFFER, mNormalData.size() * sizeof(GLfloat), mNormalData.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &mVBO_UV);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO_UV);
    glBufferData(GL_ARRAY_BUFFER, mUVData.size() * sizeof(GLfloat), mUVData.data(), GL_STATIC_DRAW);



    // Posiciones (layout 0)  desde mVBO con stride 0 porque las posiciones son contiguas
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // Normales (layout 1) desde mVBO_Normal tambien con stride 0
    glBindBuffer(GL_ARRAY_BUFFER, mVBO_Normal);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
    glEnableVertexAttribArray(1);

    // UVs (layout 2) desde mVBO_UVS con stride 0
    glBindBuffer(GL_ARRAY_BUFFER, mVBO_UV);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
    glEnableVertexAttribArray(2);

    // The one and only IBO
    glGenBuffers(1, &mEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndexData.size() * sizeof (GLuint), mIndexData.data(), GL_STATIC_DRAW);

    // Cleaning
    glBindVertexArray(0);

    if (mVAO == 0 || mVBO == 0 || mEBO == 0) {
        printf("Error: VAO (%u), VBO (%u) o EBO (%u) no fueron creados correctamente.\n", mVAO, mVBO, mEBO);
    }
}

void TexturedLitMesh::setObjectColor(glm::vec3 color) {
  // Leave this here in case we want to do later a "add Color to texture" option
    printf("Lit texture with color still not implemented\n");

    /*
    mObjectColor = color;
    mShader->Use();
    glUniform3f(mObjectColorUniform, color.x, color.y, color.z);
    */
}

void TexturedLitMesh::setIllumination(struct PointLight *light, char const* shaderName) {
    Object3D::initShader(shaderName);

    mPointLight = light;
    mShader->Use();
    // Assign extra uniforms: the light color, the light position. The texture should be managed from main loop

    int shaderID = mShader->getID();
    glUniform3fv(glGetUniformLocation(shaderID, "pointLight.position"), 1, glm::value_ptr(mPointLight->position));

    glUniform1f(glGetUniformLocation(shaderID, "pointLight.constant"), mPointLight->constant);
    glUniform1f(glGetUniformLocation(shaderID, "pointLight.linear"), mPointLight->linear);
    glUniform1f(glGetUniformLocation(shaderID, "pointLight.quadratic"), mPointLight->quadratic);

    glUniform3fv(glGetUniformLocation(shaderID, "pointLight.ambient"), 1, glm::value_ptr(mPointLight->ambient));
    glUniform3fv(glGetUniformLocation(shaderID, "pointLight.diffuse"), 1, glm::value_ptr(mPointLight->diffuse));
    glUniform3fv(glGetUniformLocation(shaderID, "pointLight.specular"), 1, glm::value_ptr(mPointLight->specular));

    mViewPositionUniform = glGetUniformLocation(mShader->getID(), "viewPosition");

    printf("TexturedLitMesh inited with shader %s. Assigned Point light and view position uniforms\n", shaderName);
}

void TexturedLitMesh::updateLightPositions() {
    mShader->Use();
    // Update the position in PointLight structure

    int shaderID = mShader->getID();
    glUniform3fv(glGetUniformLocation(shaderID, "pointLight.position"), 1, glm::value_ptr(mPointLight->position));
    
}
