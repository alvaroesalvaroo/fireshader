//
// Created by alvaro on 05/01/2026.
//

#ifndef SELECTOR_H
#define SELECTOR_H
#include "Scene.h"
#include "../Rendering/TextRenderer.h"
#include "../Rendering/SpriteRenderer.h"

class Selector : public Scene{

public:
    Selector(unsigned int width, unsigned int height);


    void Init() override;
    void Update(float dt) override;
    void ProcessInput(float dt) override;
    void Render() override;

private:
    unsigned int SelectedGame = 1; // 1-single player, 2-two players

    unsigned int Width, Height;
    TextRenderer* Text;
    TextRenderer* BangersText;
    TextRenderer* TitleText;
    SpriteRenderer* Renderer;
};


#endif //SELECTOR_H