//
// Created by alvaro on 10/04/2026.
//

#ifndef LIGHTTESTSCENE_H
#define LIGHTTESTSCENE_H
#include "Scene.h"


class Object3D;
class Camera3D;
class LightEmissor;

class LightTestScene : public Scene {
public:
    LightTestScene(int width, int height);
    ~LightTestScene() override;


    void Init() override;
    void ProcessInput(float dt) override;
    void Update(float dt) override;
    void Render(float dt) override;

private:

#define NUM_CUBES 6
    Camera3D* mCamera;
    // Object3D* mLitCube;
    Object3D* mTexturedLitCube;
    LightEmissor* mLightEmissor;

    Object3D* mGround;

};



#endif //LIGHTTESTSCENE_H