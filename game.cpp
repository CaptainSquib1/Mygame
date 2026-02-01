#include "game.h"

Game::Game(std::string title, int width, int height)
    : graphics{title, width, height} , dt{1.0/60.0}, lag{0.0}, performance_frequency{SDL_GetPerformanceFrequency()}, prev_counter{SDL_GetPerformanceCounter()} {

    // load the first "level"
    world.add_platform(320, 500, 512, 64);
    player = world.create_player();
}

void Game::input() {
    player->handle_input();
}

void Game::update() {
    Uint64 now = SDL_GetPerformanceCounter();
    lag += (now - prev_counter) / (double)performance_frequency;
    prev_counter = now;
    while (lag >= dt) {
        world.update();
        lag -= dt;
    }
}

void Game::render() {

    //clear screen
    graphics.clear();

    //draw world
    for (auto &platform : world.get_platforms()) {
        graphics.draw(platform, {0, 255, 0, 255});
    }
    auto [player_sprite, color] = player->get_sprite();
    graphics.draw(player_sprite, color);

    //update
    graphics.update();
}
