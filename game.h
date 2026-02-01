#pragma once
#include "player.h"
#include "world.h"

class Game {
public:
    Game(std::string title, int width, int height);
    void input();
    void update();
    void render();

private:
    Player* player;
    World world;
    Graphics graphics;

    // timing
    double dt;
    Uint64 performance_frequency;
    Uint64 prev_counter;
    double lag;
};