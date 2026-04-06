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
    Level level{"level_0"};
    AssetManager::get_level_details(graphics, level);


    //create player
    create_player();
    AssetManager::get_game_object_details("player", graphics, *player);
    //world
    world = new World(level, audio, player.get(), events);

    load_level("stay");
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
        if (world->end_level&&current_level!=2) {
            load_level("next");
        }
        if (world->back_level && current_level!=0) {
            load_level("previous");
        }
        if (world->treasure_level) {
            load_level("treasure");
        }
        if (world->end_treasure_level) {
            load_level("treasure_out");
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

    //enemies
    for (auto& obj : world->game_objects) {
        camera.render(obj);
    }

    //update
    graphics.update();
}

void Game::get_events() {
    events["next_level"] = new NextLevel();
    events["previous_level"] = new PreviousLevel();
    events["treasure_room"] = new TreasureRoom();
    events["treasure_room_out"] = new OutTreasureRoom();
    events["spiked"] = new Spikes();
    events["in_water"] = new Water();
}

void Game::load_level(auto direction) {
    std::string level_name;
    if (direction == "next") {
        level_name = "level_" + std::to_string(++current_level);
    }
    else if (direction == "previous") {
        level_name = "level_" + std::to_string(--current_level);
    }
    else if (direction == "treasure") {
        level_name = "treasure_room";
    }
    else if (direction == "treasure_out") {
        level_name = "level_"+std::to_string(current_level);
    }
    else {
        level_name = "level_" + std::to_string(current_level);
    }
    Level level{level_name};
    AssetManager::get_level_details(graphics, level);

    // create the world
    delete world;
    world = new World(level, audio, player.get(), events);
    // assets for objs
    for (auto& obj : world->game_objects) {
        AssetManager::get_game_object_details(obj.obj_name + "-enemy", graphics, obj);
    }
    if (direction == "previous") {
        player->physics.position = {static_cast<float>(level.player_reverse_spawn_location.x), static_cast<float>(level.player_reverse_spawn_location.y)};
    }
    else if (direction == "next") {
        player->physics.position = {static_cast<float>(level.player_spawn_location.x), static_cast<float>(level.player_spawn_location.y)};
    }
    else if (direction == "treasure_out") {
        player->physics.position = {static_cast<float>(level.out_treasure_location.x), static_cast<float>(level.out_treasure_location.y)};
    }
    else {
        player->physics.position = {static_cast<float>(level.player_spawn_location.x), static_cast<float>(level.player_spawn_location.y)};
    }
    player->fsm->current_state->on_enter(*world,*player);
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

    player = std::make_unique<GameObject>("player", fsm, input, Color{255,255,0,255});

}
