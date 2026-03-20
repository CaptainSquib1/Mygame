#include "game.h"

#include "asset_manager.h"
#include "input.h"

Game::Game(std::string title, int width, int height)
    : graphics{title, width, height},camera{graphics,64}, dt{1.0/60.0}, lag{0.0}, performance_frequency{SDL_GetPerformanceFrequency()}, prev_counter{SDL_GetPerformanceCounter()} {
    //load level
    Level level{"level_1"};
    AssetManager::get_level_details(graphics, level);

    //world
    world = new World(level);

    //player
    player = std::unique_ptr<GameObject>(world->create_player(level));
    AssetManager::get_game_object_details("player", graphics, *player);
    //spawn location's pos
    player->physics.position = {static_cast<float>(level.player_spawn_location.x), static_cast<float>(level.player_spawn_location.y)};
    player->fsm->current_state->on_enter(*world, *player);

    camera.set_location(player->physics.position);
}

void Game::handle_event(SDL_Event* event) {
    player->input->collect_discrete_event(event);
}
void Game::input() {
    player->input->get_input();
    camera.handle_input();
}

void Game::update() {
    Uint64 now = SDL_GetPerformanceCounter();
    lag += (now - prev_counter) / (float)performance_frequency;
    prev_counter = now;
    while (lag >= dt) {
        player->input->handle_input(*world, *player);
        player->update(*world,dt);
        world->update(dt);
        //put camera slightly ahead of player
        float L = length(player->physics.velocity);
        Vec displacement = 8.0f * player->physics.velocity / (1.0f + L); // change first float for distance ahead
        camera.update(player->physics.position + displacement, dt);
        lag -= dt;
    }
}

void Game::render() {

    //clear screen
    graphics.clear();

    //draw world
    camera.render(world->tilemap);

    //draw player
    camera.render(*player);

    //update
    graphics.update();
}
