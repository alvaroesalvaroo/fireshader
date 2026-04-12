//
// Created by alvaro on 10/04/2026.
//

#ifndef FIRESCENE_H
#define FIRESCENE_H
#include <vector>

#include "Scene.h"


class Object3D;
class Camera3D;
class LightEmissor;

class FireScene : public Scene {
public:
    FireScene(int width, int height);
    ~FireScene() override;

    void Init() override;
    void ProcessInput(float dt) override;
    void Update(float dt) override;
    void Render(float dt) override;

private:

    #define NUM_CUBES 6

    Camera3D* mCamera;
    // Object3D* mLitCube;
    Object3D* mTexturedLitCube;
    // LightEmissor* mLightEmissor;
    std::vector<LightEmissor*> mLights;
    std::vector<int> mLightPositionsUniforms;

    // NormalMapMesh* gGround = new NormalMapMesh();
    // Flame* gFlame = new Flame();
    Object3D* mGround;

    // TexturedLitCube* gSmokeAttemp = new TexturedLitCube();
    // NormalMapMesh* gSmoke = new NormalMapMesh();
};



#endif //FIRESCENE_H