//
// Created by alvaro on 10/04/2026.
//

#include "Material.h"

void Material::updateMaterialToShader(Shader *shader, bool useShader) {
    if (useShader) shader->Use();

    shader->SetFloat("material.ambient",   mAmbient);
    shader->SetFloat("material.diffuse",   mDiffuse);
    shader->SetFloat("material.specular",  mSpecular);
    shader->SetFloat("material.shininess", mShininess);
}