#include "game.h"

#include <iostream>

#include "asset_manager.h"
#include "input.h"
#include "states.h"
#include "keyboard_input.h"

Game::Game(std::string title, int width, int height)
    : graphics{title, width, height},camera{graphics,64}, dt{1.0/60.0}, lag{0.0}, performance_frequency{SDL_GetPerformanceFrequency()}, prev_counter{SDL_GetPerformanceCounter()} {
    //load events
    get_events();
    //load level
    Level level{"level_1"};
    AssetManager::get_level_details(graphics, level);


    //create player
    create_player();
    AssetManager::get_game_object_details("player", graphics, *player);
    //world
    world = new World(level, audio, player.get(), events);



    //spawn location's pos
    player->physics.position = {static_cast<float>(level.player_spawn_location.x), static_cast<float>(level.player_spawn_location.y)};
    player->fsm->current_state->on_enter(*world, *player);

    camera.set_location(player->physics.position);
    audio.play_sound("background", true);
}

Game::~Game() {
    delete world;
    for (auto [_,event]:events) {
        delete event;
    }
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
        if (world->end_level) {
            load_level();
        }
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

void Game::get_events() {
    events["next_level"] = new NextLevel();
    events["previous_level"] = new PreviousLevel();
    events["treasure_room"] = new TreasureRoom();
    events["spiked"] = new Spikes();
}

void Game::load_level() {
    std::string level_name = "level_" + std::to_string(++current_level);
    Level level{level_name};
    AssetManager::get_level_details(graphics, level);

    // create the world
    delete world;
    world = new World(level, audio, player.get(), events);

    player->physics.position = {static_cast<float>(level.player_spawn_location.x), static_cast<float>(level.player_spawn_location.y)};
    camera.set_location(player->physics.position);
    audio.play_sound("background", true);
}

void Game::create_player() {
    // Create FSM
    Transitions transitions = {
        // Standing to "Something"
        {{StateType::Standing, Transition::Crouch}, StateType::Crouching},
        {{StateType::Standing, Transition::Jump}, StateType::InAir},
        {{StateType::Standing, Transition::Move}, StateType::Walking},
        // Walking to "Something"
        {{StateType::Walking, Transition::Stop}, StateType::Standing},
        {{StateType::Walking, Transition::Crouch}, StateType::Crouching},
        {{StateType::Walking, Transition::Jump}, StateType::InAir},
        {{StateType::Walking, Transition::Dash}, StateType::Dashing},
        // InAir to "Something"
        {{StateType::InAir, Transition::Stop}, StateType::Standing},
        {{StateType::InAir, Transition::Swing}, StateType::Swinging},
        {{StateType::InAir, Transition::Dash}, StateType::Dashing},
        // Swinging to "Something"
        {{StateType::Swinging, Transition::Stop}, StateType::InAir},
        {{StateType::Swinging, Transition::Dash}, StateType::Dashing},
        // Dashing to "Something"
        {{StateType::Dashing,Transition::Stop},StateType::Walking},
        // Crouching to "Something"
        {{StateType::Crouching, Transition::Stop}, StateType::Standing},
        {{StateType::Crouching, Transition::Move}, StateType::Walking},
        {{StateType::Crouching, Transition::Jump}, StateType::InAir},
    };
    States states = {
        {StateType::Standing, new Standing()},
        {StateType::InAir, new InAir()},
        {StateType::Walking, new Walking()},
        {StateType::Dashing, new Dashing()},
        {StateType::Crouching, new Crouching()},
        {StateType::Swinging, new Swinging()},
    };
    FSM* fsm = new FSM(transitions, states, StateType::Standing);

    //player input
    KeyboardInput* input = new KeyboardInput;

    player = std::make_unique<GameObject>(Vec<int>{1, 1}, fsm, input, Color{255,255,0,255});

}
