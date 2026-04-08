//
// Created by alv18 on 06/05/2025.
//

#ifndef FLAME_H
#define FLAME_H
#include "../../src/Object3D.h"
#include <glad/glad.h>


class Flame : public Object3D{
public:
    Flame() {

        mNumberOfShapes = 3;
        mTimeUniform = -1;
        mUseEBO = true;
        mVertexLists.resize(mNumberOfShapes);
        mVBOList.resize(mNumberOfShapes);
    }
    void loadFlamesData(char const* filename, int numberOfShapes);
    void initShader(const char* shaderName) override;
    void render(double time, double deltaTime, Camera3D *camera) override;
private:
    void loadSingleFlameData(std::string filename, int listIndex);
    void prepareGeometryForShader();

    int mNumberOfShapes;
    int mTimeUniform;

    std::vector<std::vector<float>> mVertexLists;   // Un conjunto de mNumberOfShapes vectores para cada Shape
    std::vector<float> mUVData;
    std::vector<float> mColorData;

    // VBOs
    std::vector<GLuint> mVBOList;   // Para cada Shape de vertices
    GLuint mVBO_UV;
    GLuint mVBO_Color;


};



#endif //FLAME_H
