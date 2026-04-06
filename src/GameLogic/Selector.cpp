//
// Created by alvaro on 05/01/2026.
//

#include "Selector.h"

#include "Game.h"
#include "TwoPlayersGame.h"
#include "../ResourceManager.h"
#include "GLFW/glfw3.h"

GameObject2D* Background;
Selector::Selector(unsigned int width, unsigned int height) {
    this->Width = width;
    this->Height = height;

}


void Selector::Init() {
    // Load and init sprite shader
    Shader& spriteShader = ResourceManager::LoadShader(ShaderName::Sprite);
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width),
        static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
    spriteShader.Use().SetInteger("image", 0);
    spriteShader.SetMatrix4("projection", projection);
    // Create sprite renderer
    Renderer = new SpriteRenderer(spriteShader);

    // Create text renderer
    Text = new TextRenderer(this->Width, this->Height);
    Text->Load("assets/fonts/ocraext.TTF", 24);

    // Title text
    TitleText = new TextRenderer(Width, Height);
    TitleText->Load("assets/fonts/ElectronicHighwaySign.TTF", 50);
    // Strong text renderer
    BangersText = new TextRenderer(this->Width, this->Height);
    BangersText->Load("assets/fonts/Bangers.TTF", 50);

    ResourceManager::LoadTexture(TextureName::Background);
    ResourceManager::LoadTexture(TextureName::Arrows);
    ResourceManager::LoadTexture(TextureName::Game1P);
    ResourceManager::LoadTexture(TextureName::Game2P);
}

void Selector::Update(float dt) {
}
void Selector::ProcessInput(float dt) {
    bool changeGame = false;

    if (this->Keys[GLFW_KEY_LEFT] && !this->KeysProcessed[GLFW_KEY_LEFT]) {
        changeGame = true;
        this->KeysProcessed[GLFW_KEY_LEFT] = true;
    }
    if (this->Keys[GLFW_KEY_RIGHT] && !this->KeysProcessed[GLFW_KEY_RIGHT]) {
        changeGame = true;
        this->KeysProcessed[GLFW_KEY_RIGHT] = true;
    }
    if (this->Keys[GLFW_KEY_A] && !this->KeysProcessed[GLFW_KEY_A]) {
        changeGame = true;
        this->KeysProcessed[GLFW_KEY_A] = true;
    }
    if (this->Keys[GLFW_KEY_D] && !this->KeysProcessed[GLFW_KEY_D]) {
        changeGame = true;
        this->KeysProcessed[GLFW_KEY_D] = true;
    }

    if (changeGame) {
        SelectedGame = (SelectedGame % 2) + 1;
    }
    if (this->Keys[GLFW_KEY_ENTER] && !this->KeysProcessed[GLFW_KEY_ENTER]) {
        if (SelectedGame == 1) {
            ChangeScene(new Game(Width, Height));
        }
        else {
            ChangeScene(new TwoPlayersGame(Width, Height));
        }
    }
}

void Selector::Render() {

    // draw background
    Renderer->DrawSprite(ResourceManager::GetTexture(TextureName::Background),
        glm::vec2(0.0f, 0.0f), glm::vec2(this->Width, this->Height), 0.0f);

    // Draw title and text
    const glm::vec3 titleColor = glm::vec3(245.f/255.f, 211.f/255.f, 39.f/255.f);
    TitleText->RenderText("LOW LEVEL BRICKS", Width / 2 - 200.f, Height / 6, 1.0f, titleColor);
    if (SelectedGame == 1)
        BangersText->RenderText("Single Player", Width / 3 - 180.f, Height / 3 + 5.0f, 0.6f);
    else
        BangersText->RenderText("Two Players", Width * 2/3 + 80.f, Height / 3 + 5.0f, 0.6f);

    Text->RenderText("Arrows to select game", Width / 2 - 160.f, Height * 0.75, 1.0f);
    Text->RenderText("Supr to come back", Width / 2 - 130.f, Height * 0.80, 1.0f);
    TitleText->RenderText("ENTER TO START", Width / 2 - 156.f, Height * 0.85, 0.7f, titleColor);

    // Arrows (not cool enough)

    // Renderer->DrawSprite(ResourceManager::GetTexture(TextureName::Arrows),
    //    glm::vec2(Width/2 -75.f, Height/2 -75.f), glm::vec2(150.f, 150.f), 0.0f);

    // Game miniatures
    const float miniatureSize = 300.f;
    Renderer->DrawSprite(ResourceManager::GetTexture(TextureName::Game1P),
        glm::vec2(Width / 3 - 250.f, Height * 0.45), glm::vec2(miniatureSize, miniatureSize*9/16), 0.0f);
    Renderer->DrawSprite(ResourceManager::GetTexture(TextureName::Game2P),
        glm::vec2(Width * 2/3 , Height * 0.45), glm::vec2(miniatureSize, miniatureSize*9/16), 0.0f);


}
