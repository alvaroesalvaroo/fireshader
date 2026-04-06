/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include <algorithm>
#include <iostream>
#include <ostream>

#include "../AudioEngine.h"
#include "Game.h"

#include "Selector.h"
#include "../ResourceManager.h"
#include "../Rendering/SpriteRenderer.h"
#include "../GameObjects/BallObject.h"
#include "../GameObjects/BallObject.h"
#include "../GameObjects/BallObject.h"
#include "../GameObjects/BallObject.h"
#include "../GameObjects/BallObject.h"
#include "../GameObjects/BallObject.h"
#include "../GameObjects/BallObject.h"
#include "../GameObjects/ParticleGenerator.h"
#include "GLFW/glfw3.h"
#include "../Rendering/PostProcessor.h"
#include "../Rendering/TextRenderer.h"

GameObject2D      *Player;
const glm::vec2 PLAYER_SIZE(150.0f, 30.0f);
const float PLAYER_VELOCITY(500.0f);

BallObject      *Ball;
const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);
const float BALL_RADIUS = 12.5f;

ParticleGenerator   *Particles;
PostProcessor     *Effects;
float ShakeTime = 0.0f;

Game::Game(unsigned int width, unsigned int height)
    : State(GAME_ACTIVE), Width(width), Height(height)
{

}

Game::~Game()
{
    delete Renderer;
    delete Player;
    delete Ball;
    delete Particles;
    delete Effects;
}

void Game::Init()
{
    this->State = GAME_MENU;
    // Load textures
    ResourceManager::LoadTexture(TextureName::Block);
    ResourceManager::LoadTexture(TextureName::Background);
    ResourceManager::LoadTexture(TextureName::BlockSolid);
    ResourceManager::LoadTexture(TextureName::Ball);
    ResourceManager::LoadTexture(TextureName::Paddle);
    ResourceManager::LoadTexture(TextureName::Particle);
    ResourceManager::LoadTexture(TextureName::PowerSpeed);
    ResourceManager::LoadTexture(TextureName::PowerSticky);
    ResourceManager::LoadTexture(TextureName::PowerIncrease);
    ResourceManager::LoadTexture(TextureName::PowerConfuse);
    ResourceManager::LoadTexture(TextureName::PowerChaos);
    ResourceManager::LoadTexture(TextureName::PowerPassthrough);

    // Load audio

    AudioEngine::LoadSound("blockhit", "assets/audio/bleep.mp3");
    AudioEngine::LoadSound("paddlehit", "assets/audio/bleep.wav");
    AudioEngine::LoadSound("powerup", "assets/audio/powerup.wav");
    AudioEngine::LoadSound("solidblockhit", "assets/audio/solid.wav");

    // Load and init sprite shader
    Shader& spriteShader = ResourceManager::LoadShader(ShaderName::Sprite);
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width),
        static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
    spriteShader.Use().SetInteger("image", 0);
    spriteShader.SetMatrix4("projection", projection);

    // Load and init particle shader
    Shader& partShader = ResourceManager::LoadShader(ShaderName::Particle);
    partShader.Use().SetInteger("sprite", 0);
    partShader.SetMatrix4("projection", projection);
    Particles = new ParticleGenerator(
        partShader,
        ResourceManager::GetTexture(TextureName::Particle),
        1000
    );

    // Load post-process shader and create post-processing effects.
    ResourceManager::LoadShader(ShaderName::PostProcess);
    Effects = new PostProcessor(ResourceManager::GetShader(ShaderName::PostProcess), this->Width, this->Height);

    // Create sprite renderer
    Renderer = new SpriteRenderer(spriteShader);
    // Create text renderer
    Text = new TextRenderer(this->Width, this->Height);
    Text->Load("assets/fonts/ocraext.TTF", 24);

    LoadLevels();

    InitPlayerAndBall();
}

void Game::InitPlayerAndBall() {
    // Init player
    glm::vec2 playerPos = glm::vec2(
        this->Width / 2.0f - PLAYER_SIZE.x / 2.0f,
        this->Height - PLAYER_SIZE.y
    );
    Player = new GameObject2D(playerPos, PLAYER_SIZE, ResourceManager::GetTexture(TextureName::Paddle));
    this->Lives = 3;

    // Init ball
    glm::vec2 ballPos = playerPos + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS, -BALL_RADIUS * 2.0f);
    Ball = new BallObject(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY,ResourceManager::GetTexture(TextureName::Ball));
    ResetPlayerAndBall();
}

void Game::LoadLevels() {
    // Load game levels
    GameLevel* one = new GameLevel(); one->Load("assets/levels/one.lvl", this->Width, this->Height / 2);
    GameLevel* two = new GameLevel();; two->Load("assets/levels/two.lvl", this->Width, this->Height / 2);
    GameLevel* three = new GameLevel();; three->Load("assets/levels/three.lvl", this->Width, this->Height / 2);
    GameLevel* four = new GameLevel();; four->Load("assets/levels/four.lvl", this->Width, this->Height / 2);
    this->Levels.push_back(one);
    this->Levels.push_back(two);
    this->Levels.push_back(three);
    this->Levels.push_back(four);
    this->Level = 1;
}

void Game::Update(float dt)
{
    this->DoCollisions();


    // update powerups
    this->UpdatePowerUps(dt);

    // reduce shake time
    if (ShakeTime > 0.0f)
    {
        ShakeTime -= dt;
        if (ShakeTime <= 0.0f)
            Effects->Shake = false;
    }

    // check loss condition
    CheckLossConditions();

    // win condition
    CheckWinCondition();

    // Exit game?
    if (this->Keys[GLFW_KEY_DELETE] && !this->KeysProcessed[GLFW_KEY_DELETE])
    {
        this->KeysProcessed[GLFW_KEY_DELETE] = true;
        Scene::ChangeScene(new Selector(this->Width, this->Height));
    }
}

void Game::CheckLossConditions() {
    if (Ball->Position.y >= this->Height) // did ball reach bottom edge?
    {
        this->Lives--;
        // did the player lose all his lives? : Game over
        if (this->Lives == 0)
        {
            this->ResetLevel();
            this->State = GAME_MENU;
        }
        this->ResetPlayerAndBall();
    }
}

void Game::CheckWinCondition() {
    if (this->State == GAME_ACTIVE && this->Levels[this->Level]->IsCompleted()) {
        WinLevel();
    }
}

void Game::ProcessMenuInput() {
    if (this->Keys[GLFW_KEY_ENTER] && !this->KeysProcessed[GLFW_KEY_ENTER])
    {
        this->State = GAME_ACTIVE;
        this->KeysProcessed[GLFW_KEY_ENTER] = true;
    }
    if (this->Keys[GLFW_KEY_W] && !this->KeysProcessed[GLFW_KEY_W])
    {
        this->Level = (this->Level + 1) % (this->Levels.size());
        this->KeysProcessed[GLFW_KEY_W] = true;
    }
    if (this->Keys[GLFW_KEY_UP] && !this->KeysProcessed[GLFW_KEY_UP])
    {
        this->Level = (this->Level + 1) % (this->Levels.size());
        this->KeysProcessed[GLFW_KEY_UP] = true;
    }
    if (this->Keys[GLFW_KEY_RIGHT] && !this->KeysProcessed[GLFW_KEY_RIGHT])
    {
        this->Level = (this->Level + 1) % (this->Levels.size());
        this->KeysProcessed[GLFW_KEY_RIGHT] = true;
    }
    if (this->Keys[GLFW_KEY_S] && !this->KeysProcessed[GLFW_KEY_S])
    {
        this->Level = (this->Level + this->Levels.size() - 1) % (this->Levels.size());
        this->KeysProcessed[GLFW_KEY_S] = true;
    }
    if (this->Keys[GLFW_KEY_DOWN] && !this->KeysProcessed[GLFW_KEY_DOWN])
    {
        this->Level = (this->Level + this->Levels.size() - 1) % (this->Levels.size());
        this->KeysProcessed[GLFW_KEY_DOWN] = true;
    }
    if (this->Keys[GLFW_KEY_LEFT] && !this->KeysProcessed[GLFW_KEY_LEFT])
    {
        this->Level = (this->Level + this->Levels.size() - 1) % (this->Levels.size());
        this->KeysProcessed[GLFW_KEY_LEFT] = true;
    }

}

void Game::ProcessWinScreenInput() {
    if (this->Keys[GLFW_KEY_ENTER])
    {
        this->KeysProcessed[GLFW_KEY_ENTER] = true;
        Effects->Chaos = false;
        this->State = GAME_MENU;
    }
}
void Game::ProcessPlayerInput(float dt) {
    float velocity = PLAYER_VELOCITY * dt;
    // move playerboard
    if (this->Keys[GLFW_KEY_A])
    {
        if (Player->Position.x >= 0.0f) {
            Player->Position.x -= velocity;
            if (Ball->Stuck)
                Ball->Position.x -= velocity;
        }
    }
    if (this->Keys[GLFW_KEY_D])
    {
        if (Player->Position.x <= this->Width - Player->Size.x) {
            Player->Position.x += velocity;
            if (Ball->Stuck)
                Ball->Position.x += velocity;
        }
    }
}

void Game::UpdateBallAndParticles(float dt) {
    Ball->Move(dt, this->Width);
    // update particles
    Particles->Update(dt, *Ball, glm::vec2(Ball->Radius / 2.0f));

    if (this->Keys[GLFW_KEY_SPACE])
        Ball->Stuck = false;
}

void Game::ProcessInput(float dt)
{
    if (this->State == GAME_MENU)
    {
        ProcessMenuInput();
    }
    if (this->State == GAME_ACTIVE)
    {
        ProcessPlayerInput(dt);
        UpdateBallAndParticles(dt);
    }
    if (this->State == GAME_WIN)
    {
        ProcessWinScreenInput();
    }
}
// ======== RENDER ======== //
void Game::DrawPlayerAndBall() {
    // draw player
    Player->Draw(*Renderer);
    //draw ball
    Ball->Draw(*Renderer);

    // draw game text
    Text->RenderText("Lives:" + std::to_string(this->Lives), 5.0f, 5.0f, 1.0f);
}

void Game::DrawMenuText() {
    Text->RenderText("Press ENTER to start", 250.0f, Height / 2 + 5.0f, 1.0f);
    Text->RenderText("Press arrows to select level", 245.0f, Height / 2 + 25.0f, 0.75f);
}

void Game::DrawWinScreenText() {
    Text->RenderText(
                "You WON!!!", 320.0, Height / 2 - 20.0, 1.0, glm::vec3(0.0, 1.0, 0.0));
    Text->RenderText(
        "Press ENTER to retry or ESC to quit", 130.0, Height / 2, 1.0, glm::vec3(1.0, 1.0, 0.0)
    );
}

void Game::Render()
{
    Effects->BeginRender();

    // Game Drawing
    {
        // draw background
        Renderer->DrawSprite(ResourceManager::GetTexture(TextureName::Background),
            glm::vec2(0.0f, 0.0f), glm::vec2(this->Width, this->Height), 0.0f);
        // draw level
        this->Levels[this->Level]->Draw(*Renderer);
        // draw particles
        Particles->Draw();

        DrawPlayerAndBall();
        // draw PowerUps
        for (PowerUp &powerUp : this->PowerUps)
            if (!powerUp.Destroyed)
                powerUp.Draw(*Renderer);
    }

    if (this->State == GAME_MENU)
    {
        DrawMenuText();
    }
    if (this->State == GAME_WIN)
    {
        DrawWinScreenText();
    }

    // end rendering to postprocessing framebuffer
    Effects->EndRender();
    // render postprocessing quad
    Effects->Render(glfwGetTime());
}

void Game::ResetLevel()
{
    this->Lives = 3;
    if (this->Level == 0)
        this->Levels[0]->Load("assets/levels/one.lvl", this->Width, this->Height / 2);
    else if (this->Level == 1)
        this->Levels[1]->Load("assets/levels/two.lvl", this->Width, this->Height / 2);
    else if (this->Level == 2)
        this->Levels[2]->Load("assets/levels/three.lvl", this->Width, this->Height / 2);
    else if (this->Level == 3)
        this->Levels[3]->Load("assets/levels/four.lvl", this->Width, this->Height / 2);
}

void Game::ResetPlayerAndBall()
{
    // reset player/ball stats
    Player->Size = PLAYER_SIZE;
    Player->Position = glm::vec2(this->Width / 2.0f - PLAYER_SIZE.x / 2.0f, this->Height - PLAYER_SIZE.y);
    Ball->Reset(Player->Position + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS, -(BALL_RADIUS * 2.0f)), INITIAL_BALL_VELOCITY);
    // also disable all active powerups
    Effects->Chaos = Effects->Confuse = false;
    Ball->PassThrough = Ball->Sticky = false;
    Player->Color = glm::vec3(1.0f);
    Ball->Color = glm::vec3(1.0f);
}

// -------- POWER UPS --------
// ---------------------------
bool IsOtherPowerUpActive(std::vector<PowerUp> &powerUps, std::string type);

void Game::DeactivatePlayerAndBallPowerUp(PowerUp &powerUp) {
    if (powerUp.Type == "sticky")
    {
        if (!IsOtherPowerUpActive(this->PowerUps, "sticky"))
        {	// only reset if no other PowerUp of type sticky is active
            Ball->Sticky = false;
            Player->Color = glm::vec3(1.0f);
        }
    }
    else if (powerUp.Type == "pass-through")
    {
        if (!IsOtherPowerUpActive(this->PowerUps, "pass-through"))
        {	// only reset if no other PowerUp of type pass-through is active
            Ball->PassThrough = false;
            Ball->Color = glm::vec3(1.0f);
        }
    }
}
void Game::UpdatePowerUps(float dt)
{
    for (PowerUp &powerUp : this->PowerUps)
    {
        powerUp.Position += powerUp.Velocity * dt;
        if (powerUp.Activated)
        {
            powerUp.Duration -= dt;

            if (powerUp.Duration <= 0.0f)
            {
                // remove powerup from list (will later be removed)
                powerUp.Activated = false;
                // deactivate effects

                DeactivatePlayerAndBallPowerUp(powerUp);

                if (powerUp.Type == "confuse")
                {
                    if (!IsOtherPowerUpActive(this->PowerUps, "confuse"))
                    {	// only reset if no other PowerUp of type confuse is active
                        Effects->Confuse = false;
                    }
                }
                else if (powerUp.Type == "chaos")
                {
                    if (!IsOtherPowerUpActive(this->PowerUps, "chaos"))
                    {	// only reset if no other PowerUp of type chaos is active
                        Effects->Chaos = false;
                    }
                }
            }
        }
    }
    // Remove all PowerUps from vector that are destroyed AND !activated (thus either off the map or finished)
    // Note we use a lambda expression to remove each PowerUp which is destroyed and not activated. I HATE LAMBDA EXPRESSIONS
    this->PowerUps.erase(std::remove_if(this->PowerUps.begin(), this->PowerUps.end(),
        [](const PowerUp &powerUp) { return powerUp.Destroyed && !powerUp.Activated; }
    ), this->PowerUps.end());
}

void Game::WinLevel() {
    this->ResetLevel();
    this->ResetPlayerAndBall();
    Effects->Chaos = false;
    Effects->Confuse = false;
    this->State = GAME_WIN;
}

bool Game::ShouldSpawn(unsigned int chance) {
    unsigned int random = rand() % chance;
    return random == 0;
}

void Game::SpawnPowerUps(GameObject2D &block)
{
    int buffChance = 30;
    int debuffChance = 15;
    if (ShouldSpawn(buffChance)) // 1 in 75 chance
        this->PowerUps.push_back(
             PowerUp("speed", glm::vec3(0.5f, 0.5f, 1.0f), 0.0f, block.Position, ResourceManager::GetTexture(TextureName::PowerSpeed))
         );
    if (ShouldSpawn(buffChance))
        this->PowerUps.push_back(
            PowerUp("sticky", glm::vec3(1.0f, 0.5f, 1.0f), 20.0f, block.Position, ResourceManager::GetTexture(TextureName::PowerSticky))
        );
    if (ShouldSpawn(buffChance))
    this->PowerUps.push_back(
        PowerUp("pass-through", glm::vec3(0.5f, 1.0f, 0.5f), 10.0f, block.Position, ResourceManager::GetTexture(TextureName::PowerPassthrough))
    );
    if (ShouldSpawn(buffChance))
    this->PowerUps.push_back(
        PowerUp("pad-size-increase", glm::vec3(1.0f, 0.6f, 0.4), 0.0f, block.Position, ResourceManager::GetTexture(TextureName::PowerIncrease))
    );
    if (ShouldSpawn(debuffChance)) // negative powerups should spawn more often
    this->PowerUps.push_back(
        PowerUp("confuse", glm::vec3(1.0f, 0.3f, 0.3f), 15.0f, block.Position, ResourceManager::GetTexture(TextureName::PowerConfuse))
    );
    if (ShouldSpawn(debuffChance))
    this->PowerUps.push_back(
        PowerUp("chaos", glm::vec3(0.9f, 0.25f, 0.25f), 15.0f, block.Position, ResourceManager::GetTexture(TextureName::PowerChaos))
    );
}

void Game::ActivatePowerUp(PowerUp &powerUp, GameObject2D& player, BallObject& ball)
{
    if (powerUp.Type == "speed")
    {
        ball.Velocity *= 1.2;
    }
    else if (powerUp.Type == "sticky")
    {
        ball.Sticky = true;
        player.Color = glm::vec3(1.0f, 0.5f, 1.0f);
    }
    else if (powerUp.Type == "pass-through")
    {
        ball.PassThrough = true;
        ball.Color = glm::vec3(1.0f, 0.5f, 0.5f);
    }
    else if (powerUp.Type == "pad-size-increase")
    {
        player.Size.x += 50;
    }
    else if (powerUp.Type == "pad-size-increase-vertical")
    {
        player.Size.y += 50;
    }
    else if (powerUp.Type == "confuse")
    {
        if (!Effects->Chaos)
            Effects->Confuse = true; // only activate if chaos wasn't already active
    }
    else if (powerUp.Type == "chaos")
    {
        if (!Effects->Confuse)
            Effects->Chaos = true;
    }
}
bool IsOtherPowerUpActive(std::vector<PowerUp> &powerUps, std::string type)
{
    // Check if another PowerUp of the same type is still active
    // in which case we don't disable its effect (yet)
    for (const PowerUp &powerUp : powerUps)
    {
        if (powerUp.Activated)
            if (powerUp.Type == type)
                return true;
    }
    return false;
}


// -------- PHYSICS ---------
// --------------------------

// Finds best direction match for a ball collidin into a target
Direction Game::VectorDirection(glm::vec2 target) {
    glm::vec2 compass[] = {
        glm::vec2(0.0f, 1.0f),	// up
        glm::vec2(1.0f, 0.0f),	// right
        glm::vec2(0.0f, -1.0f),	// down
        glm::vec2(-1.0f, 0.0f)	// left
    };
    float max = 0.0f;
    unsigned int best_match = -1;
    for (unsigned int i = 0; i < 4; i++)
    {
        float dot_product = glm::dot(glm::normalize(target), compass[i]);
        if (dot_product > max)
        {
            max = dot_product;
            best_match = i;
        }
    }
    return (Direction)best_match;
}

void Game::OnBoxHit(GameObject2D &box) {
    if (!box.IsSolid) {
        AudioEngine::PlaySound("blockhit");
        box.Destroyed = true;
        this->SpawnPowerUps(box);
    }
    else {
        AudioEngine::PlaySound("solidblockhit");
        ShakeTime = 0.05f;
        Effects->Shake = true;
    }
}

// OVERWRITTED
void Game::DoBallBoxCollision(GameObject2D &box) {
    Collision collision = CheckCollision(*Ball, box);
    if (std::get<0>(collision)) // first element: boolean indicating if there is collision
    {
        // Resolve collisions, dont resolve them when pass-through is activated
        if (!(Ball->PassThrough && !box.IsSolid)) {
            ResolveBallCollision(*Ball, collision);
        }
        OnBoxHit(box);
    }
}

void Game::CheckPowerUpCollisionWithPlayer(PowerUp &powerUp) {
    if (CheckCollision(*Player, powerUp))
    {	// collided with player, now activate powerup
        AudioEngine::PlaySound("powerup");
        ActivatePowerUp(powerUp, *Player, *Ball);
        powerUp.Destroyed = true;
        powerUp.Activated = true;
    }
}
void Game::DoCollisions() {
    // Ball-boxes collision
    for (GameObject2D &box : this->Levels[this->Level]->Bricks)
    {
        if (!box.Destroyed)
        {
            DoBallBoxCollision(box);
        }
    }

    // Check for power up collisions or destruction
    for (PowerUp &powerUp : this->PowerUps)
    {
        if (!powerUp.Destroyed)
        {
            if (powerUp.Position.y >= this->Height)
                powerUp.Destroyed = true;
            CheckPowerUpCollisionWithPlayer(powerUp);
        }
    }

    CheckPaddleBallCollisions();

}
// OVERWRITTEN
void Game::CheckPaddleBallCollisions() {
    Collision result = CheckCollision(*Ball, *Player);
    if (!Ball->Stuck && std::get<0>(result))
    {
        AudioEngine::PlaySound("paddlehit");
        // power up
        Ball->Stuck = Ball->Sticky;
        ResolveBallCollision(*Ball, result);
        // check where it hit the board, and change velocity based on where it hit the board
        float centerBoard = Player->Position.x + Player->Size.x / 2.0f;
        float distance = (Ball->Position.x + Ball->Radius) - centerBoard;
        float percentage = distance / (Player->Size.x / 2.0f);
        // then move accordingly
        float strength = 2.0f;
        glm::vec2 oldVelocity = Ball->Velocity;
        Ball->Velocity.x = INITIAL_BALL_VELOCITY.x * percentage * strength;
        //Ball->Velocity.y = -Ball->Velocity.y;
        Ball->Velocity = glm::normalize(Ball->Velocity) * glm::length(oldVelocity); // keep speed consistent over both axes (multiply by length of old velocity, so total strength is not changed)

        // QUICK CORRECTION TO AVOID MULTIPLE BOUNCES
        Ball->Velocity.y = -1.0f * abs(Ball->Velocity.y);
    }
}

void Game::ResolveBallCollision(BallObject &ball, Collision collision) {
    Direction dir = std::get<1>(collision);
    glm::vec2 diff_vector = std::get<2>(collision);
    if (dir == LEFT || dir == RIGHT) // horizontal collision
    {
        ball.Velocity.x = -ball.Velocity.x; // reverse horizontal velocity
        // relocate
        float penetration = ball.Radius - std::abs(diff_vector.x);
        if (dir == LEFT)
            ball.Position.x += penetration; // move ball to right
        else
            ball.Position.x -= penetration; // move ball to left;
    }
    else // vertical collision
    {
        ball.Velocity.y = -ball.Velocity.y; // reverse vertical velocity
        // relocate
        float penetration = ball.Radius - std::abs(diff_vector.y);
        if (dir == UP)
            ball.Position.y -= penetration; // move ball back up
        else
            ball.Position.y += penetration; // move ball back down
    }
}
/*void Game::ResolveCollisions(BallObject &ball, Collision collision) {
    // collision resolution for 1 ball
    ResolveBallCollision(ball, collision);
}*/

bool Game::CheckCollision(GameObject2D &one, GameObject2D &two)// AABB - AABB collision
{
    // collision x-axis?
    bool collisionX = one.Position.x + one.Size.x >= two.Position.x &&
        two.Position.x + two.Size.x >= one.Position.x;
    // collision y-axis?
    bool collisionY = one.Position.y + one.Size.y >= two.Position.y &&
        two.Position.y + two.Size.y >= one.Position.y;
    // collision only if on both axes
    return collisionX && collisionY;
}
// Collision between circle and square
Collision Game::CheckCollision(BallObject &one, GameObject2D &two) {

    // get center point circle first
    glm::vec2 center(one.Position + one.Radius);
    // calculate AABB info (center, half-extents)
    glm::vec2 aabb_half_extents(two.Size.x / 2.0f, two.Size.y / 2.0f);
    glm::vec2 aabb_center(
        two.Position.x + aabb_half_extents.x,
        two.Position.y + aabb_half_extents.y
    );
    // get difference vector between both centers
    glm::vec2 difference = center - aabb_center;
    glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
    // add clamped value to AABB_center and we get the value of box closest to circle
    glm::vec2 closest = aabb_center + clamped;
    // retrieve vector between center circle and closest point AABB and check if length <= radius
    difference = closest - center;

    if (glm::length(difference) < one.Radius) // not <= since in that case a collision also occurs when object one exactly touches object two, which they are at the end of each collision resolution stage.
        return std::make_tuple(true, VectorDirection(difference), difference);
    else
        return std::make_tuple(false, UP, glm::vec2(0.0f, 0.0f));

}

