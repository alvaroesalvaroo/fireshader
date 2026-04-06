//
// Created by alvaro on 05/01/2026.
//

#include "TwoPlayersLevel.h"

bool TwoPlayersLevel::IsCompletedForPlayer1() {
    for (GameObject2D &tile : this->Bricks) {
        // Ignore bricks on the player2 side for deciding
        if (tile.Position.x > levelWidth /2)
            continue;

        if (!tile.IsSolid && !tile.Destroyed)
            return false;
    }
    return true;
}

bool TwoPlayersLevel::IsCompletedForPlayer2() {
    for (GameObject2D &tile : this->Bricks) {
        // Ignore bricks on the player1 side for deciding
        if (tile.Position.x < levelWidth /2)
            continue;

        if (!tile.IsSolid && !tile.Destroyed)
            return false;
    }
    return true;
}
