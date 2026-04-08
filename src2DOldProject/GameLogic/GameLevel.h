//
// Created by alvaro on 11/12/2025.
//

#ifndef GAMELEVEL_H
#define GAMELEVEL_H
#include <vector>

#include "../GameObjects/GameObject2D.h"


class GameLevel
{
public:
    // level state
    std::vector<GameObject2D> Bricks;
    // constructor
    GameLevel() { }
    virtual ~GameLevel(){}
    // loads level from file
    void Load(const char *file, unsigned int levelWidth, unsigned int levelHeight);
    // render level
    void Draw(SpriteRenderer &renderer);
    // check if the level is completed (all non-solid tiles are destroyed)
    bool IsCompleted();

private:
    // initialize level from tile data
    void init(std::vector<std::vector<unsigned int>> tileData,
              unsigned int levelWidth, unsigned int levelHeight);
};

#endif //GAMELEVEL_H