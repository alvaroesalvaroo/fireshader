//
// Created by alvaro on 04/01/2026.
//

#include "TwoPlayersGame.h"
#include "../AudioEngine.h"
#include "TwoPlayersLevel.h"
#include "../ResourceManager.h"
#include "GLFW/glfw3.h"
const glm::vec2 PLAYER_SIZE(30.0f, 150.0f);
const float PLAYER_VELOCITY(500.0f);

const float BALL_RADIUS = 12.5f;
const glm::vec2 INITIAL_BALL_VELOCITY(350.0f, -100.0f);

TwoPlayersGame::~TwoPlayersGame() {
    delete player1;
    delete player2;
    delete ball1;
    delete ball2;
    // In c++ we dont do Game::~Game();
}

void TwoPlayersGame::LoadLevels() {
    // Load game levels
    TwoPlayersLevel* one = new TwoPlayersLevel(); one->Load("assets/levels/2P-one.lvl", this->Width, this->Height);
    TwoPlayersLevel* two = new TwoPlayersLevel(); two->Load("assets/levels/2P-two.lvl", this->Width, this->Height);
    TwoPlayersLevel* three = new TwoPlayersLevel(); three->Load("assets/levels/2P-three.lvl", this->Width, this->Height);
    TwoPlayersLevel* four = new TwoPlayersLevel(); four->Load("assets/levels/2P-four.lvl", this->Width, this->Height);
    TwoPlayersLevel* five = new TwoPlayersLevel(); five->Load("assets/levels/2P-five.lvl", this->Width, this->Height);
    one->levelWidth = this->Width;
    two->levelWidth = this->Width;
    three->levelWidth = this->Width;
    four->levelWidth = this->Width;
    five->levelWidth = this->Width;

    this->Levels.push_back(one);
    this->Levels.push_back(two);
    this->Levels.push_back(three);
    this->Levels.push_back(four);
    this->Levels.push_back(five);
    /*this->Levels.push_back(three);
    this->Levels.push_back(four);*/
    this->Level = 0;
}

void TwoPlayersGame::InitPlayerAndBall() {
    // Init player 1
    glm::vec2 player1Pos = glm::vec2(
        0,
        this->Height / 2.0f - PLAYER_SIZE.y / 2.0f
    );
    ResourceManager::LoadTexture(TextureName::VerticalPaddle);
    player1 = new GameObject2D(player1Pos, PLAYER_SIZE, ResourceManager::GetTexture(TextureName::VerticalPaddle));
    this->lives1 = 3;

    // Init player 2
    glm::vec2 player2Pos = glm::vec2(
        this->Width - PLAYER_SIZE.x,
        this->Height / 2.0f - PLAYER_SIZE.y / 2.0f
    );
    player2 = new GameObject2D(player2Pos, PLAYER_SIZE, ResourceManager::GetTexture(TextureName::VerticalPaddle));
    this->lives2 = 3;

    // Init ball1 and particles
    glm::vec2 ball1Pos = player1Pos + glm::vec2(BALL_RADIUS * 2.0f, PLAYER_SIZE.y / 2.0f - BALL_RADIUS);
    ball1 = new BallObject(ball1Pos, BALL_RADIUS, INITIAL_BALL_VELOCITY,ResourceManager::GetTexture(TextureName::Ball));
    particles1 = new ParticleGenerator(
        ResourceManager::GetShader(ShaderName::Particle),
        ResourceManager::GetTexture(TextureName::Particle),
        1000
    );
    // Init ball2 and particles
    glm::vec2 ball2Pos = player2Pos + glm::vec2(-BALL_RADIUS * 2.0f, PLAYER_SIZE.y / 2.0f - BALL_RADIUS);
    ball2 = new BallObject(ball2Pos, BALL_RADIUS, -INITIAL_BALL_VELOCITY,ResourceManager::GetTexture(TextureName::Ball));
    particles2 = new ParticleGenerator(
        ResourceManager::GetShader(ShaderName::Particle),
        ResourceManager::GetTexture(TextureName::Particle),
        1000
    );
    ResetPlayerAndBall();
}

void TwoPlayersGame::DrawPlayerAndBall() {
    // draw players
    player1->Draw(*Renderer);
    player2->Draw(*Renderer);
    //draw ball
    ball1->Draw(*Renderer);
    ball2->Draw(*Renderer);

    // draw particles and lives when game is active
    if (this->State == GAME_ACTIVE) {
        particles1->Draw();
        particles2->Draw();
        Text->RenderText("P1 lives:" + std::to_string(lives1), 5.0f, 5.0f, 1.0f);
        Text->RenderText("P2 lives:" + std::to_string(lives1), static_cast<int>(this->Width) - 200.f, 5.0f, 1.0f);
    }
}

void TwoPlayersGame::DrawMenuText() {
    Text->RenderText("Press ENTER to start", 250.0f, Height / 2 + 5.0f, 1.0f);
    Text->RenderText("Press arrows to select level", 245.0f, Height / 2 + 30.0f, 0.85f);
    Text->RenderText("W↑ S↓", 100.f, Height / 4 + 5.0f, 1.0f);
    Text->RenderText("SPACE", 100.f, Height / 4 + 30.0f, 1.0f);
    Text->RenderText("ARROWS↑↓", Width - 150.f, Height / 4 + 5.0f, 1.0f);
    Text->RenderText("CTRL", Width - 150.f, Height / 4 + 30.0f, 1.0f);

}

void TwoPlayersGame::DrawWinScreenText() {
    std::string winnerText = "";
    if (winner == 1) {
        winnerText = "Player 1 wins!";
    }
    else if (winner == 2) {
        winnerText = "Player 2 wins!";
    }
    else {
        winnerText = "ITS A TIE!!"; // Can happen when both complete the game on the exact same frame
    }
    Text->RenderText(
        winnerText, 320.0, Height / 2 - 20.0, 1.0, glm::vec3(0.0, 1.0, 0.0)
    );
    Text->RenderText(
        "Press ENTER to retry or ESC to quit", 130.0, Height / 2, 1.0, glm::vec3(1.0, 1.0, 0.0)
    );
}

void TwoPlayersGame::CheckLossConditions() {
    if (ball1->Position.x < 0 - BALL_RADIUS) // did ball1 reached left edge?
    {
        this->lives1--;
        // did player 1 lose all his lives? : player 2 wins
        if (this->lives1 == 0)
        {
            winner = 2;
            this->State = GAME_WIN;
            this->ResetLevel();
            this->ResetPlayerAndBall();
        }
        else {
            this->ResetPlayer1();
        }

    }

    if (ball2->Position.x >= this->Width) // did ball2 reach right edge?
    {
        this->lives2--;
        // did player 2 lose all his lives? : player1 wins
        if (this->lives2 == 0)
        {
            winner = 1;
            this->State = GAME_WIN;
            this->ResetLevel();
            this->ResetPlayerAndBall();
        }
        else {
            this->ResetPlayer2();
        }

    }
}

void TwoPlayersGame::CheckWinCondition() {

    if (this->State == GAME_ACTIVE) {

        GameLevel* level = this->Levels[this->Level];
        TwoPlayersLevel* twoPlayerPtr = dynamic_cast<TwoPlayersLevel*>(level);

        if (twoPlayerPtr == nullptr) { // Should not happen never. Can be removed
            std::cerr << "ERROR: Level was not a 2-player level. " << std::endl;
            return;
        }
        if (twoPlayerPtr->IsCompletedForPlayer1()) {
            if (twoPlayerPtr->IsCompletedForPlayer2())
                winner = 3; // Both won on same frame! That is a tie!!
            else
                winner = 1; // Only 1 won
            WinLevel();
        }
        else if (twoPlayerPtr->IsCompletedForPlayer2()) {
            winner = 2; // Only 2 won
            WinLevel();
        }
    }
}

void TwoPlayersGame::ProcessPlayerInput(float dt) {
    float velocity = PLAYER_VELOCITY * dt;
    // Move player1
    if (this->Keys[GLFW_KEY_W])
    {
        if (player1->Position.y >= 0.0f) {
            player1->Position.y -= velocity;
            if (ball1->Stuck)
                ball1->Position.y -= velocity;
        }
    }
    if (this->Keys[GLFW_KEY_S])
    {
        if (player1->Position.y <= this->Height - player1->Size.y) {
            player1->Position.y += velocity;
            if (ball1->Stuck)
                ball1->Position.y += velocity;
        }
    }
    // Move player2
    if (this->Keys[GLFW_KEY_UP])
    {
        if (player2->Position.y >= 0.0f) {
            player2->Position.y -= velocity;
            if (ball2->Stuck)
                ball2->Position.y -= velocity;
        }
    }
    if (this->Keys[GLFW_KEY_DOWN])
    {
        if (player2->Position.y <= this->Height - player2->Size.y) {
            player2->Position.y += velocity;
            if (ball2->Stuck)
                ball2->Position.y += velocity;
        }
    }
}

void TwoPlayersGame::UpdateBallAndParticles(float dt) {
    ball1->MoveLaterally(dt, this->Height);
    ball2->MoveLaterally(dt, this->Height);

    particles1->Update(dt, *ball1, glm::vec2(ball1->Radius / 2.0f));
    particles2->Update(dt, *ball2, glm::vec2(ball2->Radius / 2.0f));


    if (this->Keys[GLFW_KEY_SPACE])
        ball1->Stuck = false;

    if (this->Keys[GLFW_KEY_RIGHT_CONTROL])
        ball2->Stuck = false;
}

void TwoPlayersGame::ResetLevel() {
    this->lives1 = 3;
    this->lives2 = 3;
    if (this->Level == 0)
        this->Levels[0]->Load("assets/levels/2P-one.lvl", this->Width, this->Height);
    else if (this->Level == 1)
        this->Levels[1]->Load("assets/levels/2P-two.lvl", this->Width, this->Height);
    else if (this->Level == 2)
        this->Levels[2]->Load("assets/levels/2P-three.lvl", this->Width, this->Height);
    else if (this->Level == 3)
        this->Levels[3]->Load("assets/levels/2P-four.lvl", this->Width, this->Height);
    else if (this->Level == 4)
        this->Levels[3]->Load("assets/levels/2P-five.lvl", this->Width, this->Height);
}

void TwoPlayersGame::ResetPlayerAndBall() {
    // Reset both players
    ResetPlayer1();
    ResetPlayer2();
    // Also reset chaos
    Effects->Chaos = Effects->Confuse = false;
}

void TwoPlayersGame::ResetPlayer1() {
    player1->Size = PLAYER_SIZE;
    glm::vec2 player1Pos = glm::vec2(0,this->Height / 2.0f - PLAYER_SIZE.y / 2.0f);
    player1->Position = player1Pos;
    player1->Color = glm::vec3(1.0f);
    glm::vec2 ball1Pos = player1Pos + glm::vec2(BALL_RADIUS * 2.0f, PLAYER_SIZE.y / 2.0f - BALL_RADIUS);
    ball1->Reset(ball1Pos, INITIAL_BALL_VELOCITY);
    ball1->Color = glm::vec3(1.0f);
    ball1->PassThrough = ball1->Sticky = false;
}

void TwoPlayersGame::ResetPlayer2() {
    player2->Size = PLAYER_SIZE;
    glm::vec2 player2Pos = glm::vec2(this->Width - PLAYER_SIZE.x,this->Height / 2.0f - PLAYER_SIZE.y / 2.0f);
    player2->Position = player2Pos;
    player2->Color = glm::vec3(1.0f);

    glm::vec2 ball2Pos = player2Pos + glm::vec2(-BALL_RADIUS * 2.0f, PLAYER_SIZE.y / 2.0f - BALL_RADIUS);
    ball2->Reset(ball2Pos, -INITIAL_BALL_VELOCITY);
    ball2->Color = glm::vec3(1.0f);
    ball2->PassThrough = ball2->Sticky = false;
}

void TwoPlayersGame::SpawnPowerUps(GameObject2D &block) {
    int buffChance = 30;
    int debuffChance = 15;
    bool isPlayer1Buff = block.Position.x < this->Width / 2;
    glm::vec2 velocity = glm::vec2(isPlayer1Buff ? -150.f : 150.f , 0.0f);
    if (ShouldSpawn(buffChance)) // 1 in 75 chance
        this->PowerUps.push_back(
             PowerUp("speed", glm::vec3(0.5f, 0.5f, 1.0f), 0.0f, block.Position, ResourceManager::GetTexture(TextureName::PowerSpeed), velocity)
         );
    if (ShouldSpawn(buffChance))
        this->PowerUps.push_back(
            PowerUp("sticky", glm::vec3(1.0f, 0.5f, 1.0f), 20.0f, block.Position, ResourceManager::GetTexture(TextureName::PowerSticky), velocity)
        );
    if (ShouldSpawn(buffChance))
        this->PowerUps.push_back(
            PowerUp("pass-through", glm::vec3(0.5f, 1.0f, 0.5f), 10.0f, block.Position, ResourceManager::GetTexture(TextureName::PowerPassthrough), velocity)
        );
    if (ShouldSpawn(buffChance))
        this->PowerUps.push_back(
            PowerUp("pad-size-increase-vertical", glm::vec3(1.0f, 0.6f, 0.4), 0.0f, block.Position, ResourceManager::GetTexture(TextureName::PowerIncrease), velocity)
        );
    if (ShouldSpawn(debuffChance)) // negative powerups should spawn more often
        this->PowerUps.push_back(
            PowerUp("confuse", glm::vec3(1.0f, 0.3f, 0.3f), 15.0f, block.Position, ResourceManager::GetTexture(TextureName::PowerConfuse), velocity)
        );
    if (ShouldSpawn(debuffChance))
        this->PowerUps.push_back(
            PowerUp("chaos", glm::vec3(0.9f, 0.25f, 0.25f), 15.0f, block.Position, ResourceManager::GetTexture(TextureName::PowerChaos), velocity)
        );
}

bool TwoPlayersGame::IsOtherPowerUpActive(std::vector<PowerUp> &powerUps, std::string type, BallObject &ball) {
    for (const PowerUp &powerUp : powerUps)
    {
        if (powerUp.Activated) {
            // If ball and powerup are on the same side of the field, that means power up is applicated to the same ball
            if (ball.Position.x > this->Width / 2 && powerUp.Position.x > this->Width / 2)
                return true;
            if (ball.Position.x > this->Width / 2 && powerUp.Position.x > this->Width / 2)
                return true;
        }
    }
    return false;
}

void TwoPlayersGame::DeactivatePlayerAndBallPowerUp(PowerUp &powerUp) {
    // Check sticky on both balls
    if (powerUp.Type == "sticky")
    {
        if (!IsOtherPowerUpActive(this->PowerUps, "sticky", *ball1))
        {	// only reset if no other PowerUp of type sticky is active
            ball1->Sticky = false;
            player1->Color = glm::vec3(1.0f);
        }
        if (!IsOtherPowerUpActive(this->PowerUps, "sticky", *ball2))
        {
            ball2->Sticky = false;
            player2->Color = glm::vec3(1.0f);
        }

    }
    // Check pass-through on both balls
    else if (powerUp.Type == "pass-through")
    {
        if (!IsOtherPowerUpActive(this->PowerUps, "pass-through", *ball1))
        {	// only reset if no other PowerUp of type pass-through is active
            ball1->PassThrough = false;
            ball1->Color = glm::vec3(1.0f);
        }
        if (!IsOtherPowerUpActive(this->PowerUps, "pass-through", *ball2)) {
            ball2->PassThrough = false;
            ball2->Color = glm::vec3(1.0f);
        }
    }

}

void TwoPlayersGame::DoBallBoxCollision(GameObject2D &box) {
    // Ball1
    Collision collision = CheckCollision(*ball1, box);
    if (std::get<0>(collision)) // first element of collision: boolean indicating if there is collision
    {
        // Resolve collisions, don't resolve them when pass-through is activated
        if (!(ball1->PassThrough && !box.IsSolid)) {
            ResolveBallCollision(*ball1, collision);
        }
        OnBoxHit(box);
    }
    // Ball2
    collision = CheckCollision(*ball2, box);
    if (std::get<0>(collision)) // first element of collision: boolean indicating if there is collision
    {
        // Resolve collisions, don't resolve them when pass-through is activated
        if (!(ball2->PassThrough && !box.IsSolid)) {
            ResolveBallCollision(*ball2, collision);
        }
        OnBoxHit(box);
    }
}

void TwoPlayersGame::CheckPowerUpCollisionWithPlayer(PowerUp &powerUp) {
    // Player 1
    if (CheckCollision(*player1, powerUp))
    {	// collided with player, now activate powerup
        AudioEngine::PlaySound("powerup");
        ActivatePowerUp(powerUp, *player1, *ball1);
        powerUp.Destroyed = true;
        powerUp.Activated = true;
    }
    // Player 2
    if (CheckCollision(*player2, powerUp))
    {	// collided with player, now activate powerup
        AudioEngine::PlaySound("powerup");
        ActivatePowerUp(powerUp, *player2, *ball2);
        powerUp.Destroyed = true;
        powerUp.Activated = true;
    }
}
void TwoPlayersGame::CheckPaddleBallCollisions() {
    // Ball1 with player1
    Collision col = CheckCollision(*ball1, *player1);
    if (!ball1->Stuck && std::get<0>(col))
    {
        // REALOCATE BALL: ALLOWS US TO AVOID PERMANENT STUCK BALL WHEN STICKY
        ResolveBallCollision(*ball1, col);

        AudioEngine::PlaySound("paddlehit");
        // sticky power up
        ball1->Stuck = ball1->Sticky;

        // check where it hit the board, and change velocity based on where it hit the board
        float centerBoard = player1->Position.y + player1->Size.y / 2.0f;
        float distance = (ball1->Position.y + ball1->Radius) - centerBoard;
        float percentage = distance / (player1->Size.y / 2.0f);
        // then move accordingly
        float strength = 2.0f;
        glm::vec2 oldVelocity = ball1->Velocity;
        ball1->Velocity.y = - INITIAL_BALL_VELOCITY.y * percentage * strength;
        ball1->Velocity = glm::normalize(ball1->Velocity) * glm::length(oldVelocity); // keep speed consistent over both axes (multiply by length of old velocity, so total strength is not changed)

        // Correction to avoid multiple colisions: ball must has POSITIVE x velocity
        ball1->Velocity.x = + abs(ball1->Velocity.x);
    }

    // Ball2 with player2
    col = CheckCollision(*ball2, *player2);
    if (!ball2->Stuck && std::get<0>(col))
    {
        AudioEngine::PlaySound("paddlehit");
        ResolveBallCollision(*ball2, col);
        // sticky power up
        ball2->Stuck = ball2->Sticky;


        // check where it hit the board, and change velocity based on where it hit the board
        float centerBoard = player2->Position.y + player2->Size.y / 2.0f;
        float distance = (ball2->Position.y + ball2->Radius) - centerBoard;
        float percentage = distance / (player2->Size.y / 2.0f);
        // then move accordingly
        float strength = 2.0f;
        glm::vec2 oldVelocity = ball2->Velocity;
        ball2->Velocity.y = - INITIAL_BALL_VELOCITY.y * percentage * strength;
        //ball1->Velocity.y = -ball1->Velocity.y;
        ball2->Velocity = glm::normalize(ball2->Velocity) * glm::length(oldVelocity); // keep speed consistent over both axes (multiply by length of old velocity, so total strength is not changed)

        // Correction to avoid multiple colisions: ball must has NEGATIVE x velocity
        ball2->Velocity.x = -1.0f * abs(ball2->Velocity.x);
    }


}


