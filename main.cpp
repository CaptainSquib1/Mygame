#define SDL_MAIN_USE_CALLBACKS 1
#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "game.h"
#include "NetworkInput.h"

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
    // std::string boot_mode;
    // std::cout << "Choose start mode:\nnetwork\nlocal";
    // std::cin >> boot_mode;
    // if (boot_mode == "network") {
    //     NetworkInput network_input;
    //     network_input.create_connection();
    // }
    // else {} //for future use

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    auto game = new Game("Yay Game Worke", 1920, 1080);
    *appstate = game;

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;
    }

    auto game = static_cast<Game*>(appstate);
    game->handle_event(event);

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate) {
    auto game = static_cast<Game*>(appstate);

    game->input();
    // game->network_input();
    game->update();
    // game->network_update();
    game->render();


    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {
    auto game = static_cast<Game*>(appstate);
    // game->network_close();
    delete game;

}