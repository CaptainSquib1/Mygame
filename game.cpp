#include "game.h"

Game::Game(std::string title, int width, int height)
    : graphics{title, width, height} {

    // load the first "level"
    world.add_platform(320, 500, 512, 64);
    player = world.create_player();
}

void Game::input() {
    player->handle_input();
}

void Game::update() {
    world.update();
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
