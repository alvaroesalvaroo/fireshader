//
// Created by alvaro on 04/01/2026.
//

#ifndef TWOPLAYERSGAME_H
#define TWOPLAYERSGAME_H
#include "Game.h"


class TwoPlayersGame : public Game {
public:
    TwoPlayersGame(unsigned int width, unsigned int height)
    : Game(width, height) { }
    ~TwoPlayersGame();
private:
    GameObject2D* player1;
    GameObject2D* player2;
    int lives1;
    int lives2;
    BallObject* ball1;
    BallObject* ball2;
    ParticleGenerator* particles1;
    ParticleGenerator* particles2;

    int winner = -1; // can be 1 or 2
protected:

    void LoadLevels() override;
    void InitPlayerAndBall() override;

    void DrawPlayerAndBall() override;

    void DrawMenuText() override;
    void DrawWinScreenText() override;

    void CheckLossConditions() override;
    void CheckWinCondition() override;

    void ProcessPlayerInput(float dt) override;
    void UpdateBallAndParticles(float dt) override;

    void ResetLevel() override;
    void ResetPlayerAndBall() override;

    void ResetPlayer1();

    void ResetPlayer2();

    // Spawn power ups
    void SpawnPowerUps(GameObject2D &block) override;
    bool IsOtherPowerUpActive(std::vector<PowerUp> &powerUps, std::string type, BallObject &ball);
    void DeactivatePlayerAndBallPowerUp(PowerUp &powerUp) override;

    // Collisions
    void DoBallBoxCollision(GameObject2D &box) override;
    void CheckPowerUpCollisionWithPlayer(PowerUp &powerUp) override;
    void CheckPaddleBallCollisions() override;

};


#endif //TWOPLAYERSGAME_H
