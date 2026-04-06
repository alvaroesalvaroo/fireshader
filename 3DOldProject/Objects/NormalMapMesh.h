//
// Created by alv18 on 04/05/2025.
//

#ifndef NormalMapMesh_H
#define NormalMapMesh_H
#include "TexturedLitMesh.h"

class NormalMapMesh : public TexturedLitMesh{
public:
    NormalMapMesh() : TexturedLitMesh() {
    }
    int loadNormalTexture(char const* filename);

    void render(double time, double deltaTime, Camera3D *camera) override;

private:

    char const *mShaderName = "LitNormalTexturedMatrix";
    GLuint mNormalMapTexture;

};



#endif //NormalMapMesh_H
