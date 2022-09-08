#pragma once

#include "game.h"

class GameUI
{
public:
    GameUI(Game& p_game) : s_game(p_game) {}

    void Update(float elapsed);

private:
    Game& s_game;
};

