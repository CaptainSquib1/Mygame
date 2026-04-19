#pragma once
#include "camera.h"
#include "game_object.h"
#include "world.h"
#include "audio.h"
#include "events.h"


class Client_Game {
public:
    Client_Game(std::string title, int width, int height);
    ~Client_Game();
    void handle_event(SDL_Event* event);
    void input();
    void render();

private:
    std::unique_ptr<GameObject> player;
    std::unique_ptr<GameObject> network_player;
    World* world = nullptr;
    Graphics graphics;

    // timing
    float dt;
    Uint64 performance_frequency;
    Uint64 prev_counter;
    float lag;

    Camera camera;
    Audio audio;

};
