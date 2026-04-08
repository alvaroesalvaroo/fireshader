//
// Created by alv18 on 04/05/2025.
//

#ifndef TEXTUREDUNLITCUBE_H
#define TEXTUREDUNLITCUBE_H
#include "Object3D.h"

class TexturedUnlitCube : public Object3D {
public:
    TexturedUnlitCube() : Object3D() {
    }
    void render(double time, double deltaTime, Camera3D *camera) override;
protected:
    std::string mShaderName = "TextureMatrix";   // Does not need color or ilumination
};



#endif //TEXTUREDUNLITCUBE_H
