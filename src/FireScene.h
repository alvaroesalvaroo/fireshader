//
// Created by alvaro on 10/04/2026.
//

#ifndef FIRESCENE_H
#define FIRESCENE_H
#include <vector>

#include "PostProcessor.h"
#include "Scene.h"


class Spark;
class Object3D;
class Camera3D;
class LightEmissor;

inline char const* GroundTextureFilename = "textures/rocky_gravel_diff_1k.jpg";
inline char const* GroundNormalTextureFilename = "textures/rocky_gravel_nor_gl_1k.jpg";
inline char const* SmokeTextureName = "textures/smoke4.png";
inline char const* NoiseTextureName = "textures/perlin.png";

inline char const* SparkMeshFilename = "mesh/Spark.obj";
inline char const* SparkMeshFilename2 = "mesh/Spark2.obj";
inline char const* GroundMeshFilename = "mesh/CurvedPlane2.obj";
inline char const* QuadFilename = "mesh/Quad.obj";
// inline char const* FlameMeshFilename = "mesh/Llama";


inline char const* SmokeShaderName = "Smoke";
inline char const* GroundShaderName = "NormalmapMultilit";
inline char const* FlameShaderName = "Flame";
inline const char* DistorsionShaderName = "postprocess";

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
    std::vector<LightEmissor*> mFakeSparks;
    std::vector<int> mLightPositionsUniforms; // Optimización innecesaria

    PostProcessor* mEffects;
    Object3D* mSmoke;
    Object3D* mGround;

    // TexturedLitCube* gSmokeAttemp = new TexturedLitCube();
    // NormalMapMesh* gSmoke = new NormalMapMesh();
};

#endif //FIRESCENE_H