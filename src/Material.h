//
// Created by alvaro on 10/04/2026.
//

#ifndef MATERIAL_H
#define MATERIAL_H

#include "Shader.h"

class Material {
public:
    Material(float ambient = 0.1f, float diffuse = 0.8f, float specular = 0.4f, float shininess = 32.0f)
        : mAmbient(ambient), mDiffuse(diffuse), mSpecular(specular), mShininess(shininess) {}
    float mAmbient;
    float mDiffuse;
    float mSpecular;
    float mShininess;

    void updateMaterialToShader(Shader* shader, bool useShader = false);


};


#endif //MATERIAL_H