#include "client_game.h"


#include "game.h"

#include <iostream>

#include "asset_manager.h"
#include "input.h"
#include "states.h"
#include "keyboard_input.h"

Client_Game::Client_Game(std::string title, int width, int height)
    : graphics{title, width, height},camera{graphics,64}, dt{1.0/60.0}, lag{0.0}, performance_frequency{SDL_GetPerformanceFrequency()}, prev_counter{SDL_GetPerformanceCounter()} {
    //load events
    get_events();
    //load level

    //create player
    create_player();
    AssetManager::get_game_object_details("player", graphics, *player);

    load_level("stay");
}

Client_Game::~Client_Game() {
    delete world;
    for (auto [_,event]:events) {
        delete event;
    }
}

void Client_Game::handle_event(SDL_Event* event) {
    player->input->collect_discrete_event(event);
}
void Client_Game::input() {
    player->input->get_input();
    camera.handle_input();
}


void Client_Game::render() {

    //clear screen
    graphics.clear();

    //draw world
    camera.render(world->tilemap);

    //draw player
    camera.render(*player);

    //enemies
    for (auto& obj : world->game_objects) {
        camera.render(*obj);
    }

    //update
    graphics.update();
}




