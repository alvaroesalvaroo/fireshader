/*******************************************************************
** This code has small modifications from Breakout by Joey de Vries:
**** Uses enum classes instead of asking for a specific literal path
**** for loading texture and shaders resources
******************************************************************/

#pragma once
#include "GameLevel.h"
#include "../../src/Scene.h"
#include "../GameObjects/BallObject.h"
#include "../GameObjects/ParticleGenerator.h"
#include "../GameObjects/PowerUp.h"
#include "glad/glad.h"
#include "../Rendering/PostProcessor.h"
#include "../Rendering/TextRenderer.h"

// Represents the current state of the game
enum GameState {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};

enum Direction {
    UP,
    RIGHT,
    DOWN,
    LEFT
};

typedef std::tuple<bool, Direction, glm::vec2> Collision; // <collision?, what direction?, penetration>

class Game : public Scene
{
public:

    GLuint Width, Height;
    // Constructor/Destructor
    Game(GLuint width, GLuint height);
    ~Game();

    // Initialize game state (load all shaders/textures/levels)
    void Init() override;

    // GameLoop
    void Update(GLfloat dt) override;
    void ProcessInput(GLfloat dt) override;
    void Render() override;

protected:


    virtual void DrawPlayerAndBall();
    virtual void DrawMenuText();
    virtual void DrawWinScreenText();


    virtual void CheckLossConditions();
    virtual void CheckWinCondition();

    void ProcessMenuInput(); // not virtual for now
    void ProcessWinScreenInput(); // not virtual for now
    virtual void ProcessPlayerInput(float dt);
    virtual void UpdateBallAndParticles(float dt);

    virtual void ResetLevel();
    virtual void ResetPlayerAndBall();

    // Powerups
    virtual void SpawnPowerUps(GameObject2D &block);
    virtual void DeactivatePlayerAndBallPowerUp(PowerUp &powerUp);

    // Collisions
    virtual void DoBallBoxCollision(GameObject2D &box);
    virtual void CheckPowerUpCollisionWithPlayer(PowerUp &powerUp);
    virtual void CheckPaddleBallCollisions();


    std::vector<GameLevel*> Levels;
    unsigned int           Level;

    void WinLevel();

    // Powerups
    bool ShouldSpawn(unsigned int chance);
    void ActivatePowerUp(PowerUp &powerUp, GameObject2D &player, BallObject &ball);

    // Collisions
    Direction VectorDirection(glm::vec2 target);
    void OnBoxHit(GameObject2D &box);
    Collision CheckCollision(BallObject &one, GameObject2D &two);
    bool CheckCollision(GameObject2D &one, GameObject2D &two); // 2-quads collision
    void ResolveBallCollision(BallObject &ball, Collision collision);

    SpriteRenderer  *Renderer;
    TextRenderer    *Text;

    ParticleGenerator   *Particles;
    PostProcessor     *Effects;
    float ShakeTime = 0.0f;

};
