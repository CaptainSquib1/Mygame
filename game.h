#pragma once
#include "camera.h"
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
    float dt;
    Uint64 performance_frequency;
    Uint64 prev_counter;
    float lag;
    Camera camera;
};