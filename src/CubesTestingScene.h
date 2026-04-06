//
// Created by alvaro on 06/04/2026.
//

#ifndef CUBESTESTINGSCENE_H
#define CUBESTESTINGSCENE_H

#include "Scene.h"

// Forward declarations para evitar dependencias circulares innecesarias en el header
class TexturedUnlitCube;
class LitCube;
class TexturedLitPlane;
class LightEmissorCube;
class Camera3D;

class CubesTestingScene : public Scene {
public:
    CubesTestingScene(int width, int height);
    ~CubesTestingScene();

    void Init() override;
    void ProcessInput(float dt) override;
    void Update(float dt) override;
    void Render() override;

private:
    int screenWidth, screenHeight;
    const char* mTextureFilename = "textures/floor.jpg";

#define NUM_CUBES 6
    Camera3D* mCamera;
    TexturedUnlitCube* mCubes[NUM_CUBES];
    LitCube* mLitCube;
    TexturedLitPlane* mTexturedLitCube;
    LightEmissorCube* mLightEmissor;
};

#endif //CUBESTESTINGSCENE_H