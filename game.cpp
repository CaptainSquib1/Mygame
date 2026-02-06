#include "game.h"

Game::Game(std::string title, int width, int height)
    : graphics{title, width, height}, world{31,11},camera{graphics,64}, dt{1.0/60.0}, lag{0.0}, performance_frequency{SDL_GetPerformanceFrequency()}, prev_counter{SDL_GetPerformanceCounter()} {

    // load the first "level"
    // boundary walls
    world.add_platform(0,0,30,1);
    world.add_platform(0,0,1,10);
    world.add_platform(30,0,1,10);
    world.add_platform(0,10,31,1);

    // platforms
    world.add_platform(3,7,4,1);
    world.add_platform(13,4,6,1);

    player = world.create_player();
}

void Game::input() {
    player->handle_input();
}

void Game::update() {
    Uint64 now = SDL_GetPerformanceCounter();
    lag += (now - prev_counter) / (float)performance_frequency;
    prev_counter = now;
    while (lag >= dt) {
        world.update(dt);
        camera.update(player->position, dt);
        lag -= dt;
    }
}

void Game::render() {

    //clear screen
    graphics.clear();

    //draw world
    camera.render(world.tilemap);

    //draw player
    auto [player_position, color] = player->get_sprite();
    camera.render(player_position, color);

    //update
    graphics.update();
}
