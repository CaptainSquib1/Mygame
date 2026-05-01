#pragma once
#include "camera.h"
#include "game_object.h"
#include "world.h"
#include "fsm.h"
#include "audio.h"
#include "events.h"


enum class GameMode{Playing, GameOver, Win};

class Game {
public:
    Game(std::string title, int width, int height);
    ~Game();
    void handle_event(SDL_Event* event);
    void input();
    void update();
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

    //events
    Events events;
    void get_events();

    //level help
    void create_player();
    void create_network_player();
    void update_enemy(GameObject& obj);
    int current_level{0};
    void load_level(auto direction);

    // void network_update


    GameMode mode{GameMode::Playing};

};
