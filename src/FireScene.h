//
// Created by alvaro on 10/04/2026.
//

#ifndef FIRESCENE_H
#define FIRESCENE_H
#include <vector>

#include "Scene.h"


class Spark;
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

    Camera3D* mCamera;

    std::vector<LightEmissor*> mLights; // They will be initialized as "sparks"

    std::vector<int> mLightPositionsUniforms; // Optimización innecesaria

    Object3D* mSmoke;
    Object3D* mGround;

    // TexturedLitCube* gSmokeAttemp = new TexturedLitCube();
    // NormalMapMesh* gSmoke = new NormalMapMesh();
};

#endif //FIRESCENE_H