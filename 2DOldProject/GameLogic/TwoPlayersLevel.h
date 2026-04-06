//
// Created by alvaro on 05/01/2026.
//

#ifndef TWOPLAYERSLEVEL_H
#define TWOPLAYERSLEVEL_H
#include "GameLevel.h"

class TwoPlayersLevel : public GameLevel {

public:
    int levelWidth;
    bool IsCompletedForPlayer1();
    bool IsCompletedForPlayer2();
};


#endif //TWOPLAYERSLEVEL_H