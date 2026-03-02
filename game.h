#pragma once
#include "camera.h"
#include "game_object.h"
#include "world.h"
#include "fsm.h"

class Game {
public:
    Game(std::string title, int width, int height);
    void handle_event(SDL_Event* event);
    void input();
    void update();
    void render();

private:
    GameObject* player;
    World world;
    Graphics graphics;

    // timing
    float dt;
    Uint64 performance_frequency;
    Uint64 prev_counter;
    float lag;
    Camera camera;
};