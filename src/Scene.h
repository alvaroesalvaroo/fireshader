#ifndef SCENE_H
#define SCENE_H

class Scene {
public:
    static void ChangeScene(Scene* scene) {

        if (CurrentScene != nullptr) {
            delete CurrentScene; // Se viene crasheo
        }
        // Reset keys state
        for (int i = 0; i < 1024; ++i) {
            scene->Keys[i] = false;
            scene->KeysProcessed[i] = false;
        }
        // scene->Init();
        CurrentScene = scene;
    }
    static inline Scene* CurrentScene;

    Scene(int width, int height) {screenHeight = height; screenWidth = width;}
    virtual ~Scene() {}

    // Métodos virtuales puros (= 0)
    virtual void Init() = 0;
    virtual void ProcessInput(float dt) = 0;
    virtual void Update(float dt) = 0;
    virtual void Render(float dt) = 0;

    bool Keys[1024];
    bool KeysProcessed[1024];
    float mouseDeltaX;
    float mouseDeltaY;
protected:
    int screenWidth, screenHeight;

};

#endif