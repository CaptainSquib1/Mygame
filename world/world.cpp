#include "world.h"
#include <SDL3/SDL_rect.h>
#include <algorithm>
#include "game_object.h"
#include "fsm.h"
#include "states.h"
#include "level.h"
#include "vec.h"
#include "physics.h"
#include "keyboard_input.h"
#include "audio.h"

World::World(const Level& level, Audio& audio) : tilemap{level.width, level.height}, audio{&audio}{
    load_level(level);
}

void World::add_platform(float x, float y, float width, float height) {
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            tilemap(x+j,y+i) = Tile{};
        }
    }
}

void World::move_to(Vec<float> &position, const Vec<int> &size, Vec<float> &velocity) {
    // test for collisions on the bottom or top first

    Vec<float> topRight = {position.x + size.x, position.y + size.y};
    Vec<float> topLeft = {position.x, position.y + size.y};

    Vec<float> topmiddle = {topLeft.x + .5f, topLeft.y};
    if ((collides(topRight) && collides(topLeft)) || collides(topmiddle)) {
        position.y = floor(position.y);
        velocity.y = 0;
    }


    Vec<float> bottomRight = {position.x + size.x, position.y};
    Vec<float> bottomLeft = position;
    Vec<float> bottommiddle = {bottomLeft.x + .5f, bottomLeft.y};
    if ((collides(bottomRight) && collides(bottomLeft)) || collides(bottommiddle)) {
        position.y = ceil(position.y);
        velocity.y = 0;
    }


    // then test for collisions on the left and right sides

    Vec<float> leftmiddle = {bottomLeft.x,bottomLeft.y + .5f};
    if ((collides(topLeft) && collides(bottomLeft)) || collides(leftmiddle)) {
        position.x = ceil(position.x);
        velocity.x = 0;
    }
    Vec<float> rightmiddle = {bottomRight.x, bottomRight.y + .5f};
    if ((collides(topRight) && collides(bottomRight)) || collides(rightmiddle)) {
        position.x = floor(position.x);
        velocity.x = 0;
    }

    // now test each corner
    if (collides(bottomLeft)) {
        auto dx = ceil(position.x) - position.x;
        auto dy = ceil(position.y) - position.y;
        if (dx > dy) {
            position.y = ceil(position.y);
            velocity.y = 0;
        }
        else {
            position.x = ceil(position.x);
            velocity.x = 0;
        }
    }
    if (collides(topLeft)) {
        auto dx = ceil(position.x) - position.x;
        auto dy = floor(position.y + size.y) - (position.y + size.y);
        if (std::abs(dx) > std::abs(dy)) {
            position.y = dy + position.y;
            velocity.y = 0;
        }
        else {
            position.x = dx + position.x;
            velocity.x = 0;
        }
    }
    if (collides(topRight)) {
        auto dx = floor(position.x + size.x) - (position.x + size.x);
        auto dy = floor(position.y + size.y) - (position.y + size.y);
        if (dx < dy) {
            position.y = dy + position.y;
            velocity.y = 0;
        }
        else {
            position.x = dx + position.x;
            velocity.x = 0;
        }
    }

    if (collides(bottomRight)) {
        auto dx = floor(position.x + size.x) - (position.x + size.x);
        auto dy = ceil(position.y) - (position.y);
        if (std::abs(dx) > std::abs(dy)) {
            position.y = position.y + dy;
            velocity.y = 0;
        }
        else {
            position.x = position.x + dx;
            velocity.x = 0;
        }
    }
}

bool World::collides(const Vec<float>& position) const {
    int x = std::floor(position.x);
    int y = std::floor(position.y);
    return tilemap(x,y).blocking;
}

GameObject* World::create_player(const Level& level) {
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


    player = new GameObject(Vec<float>{static_cast<float>(level.player_spawn_location.x), static_cast<float>(level.player_spawn_location.y)}, Vec<int>{1, 1}, *this, fsm, input, Color{255,255,0,255});

    return player;
}

void World::update(float dt) {
    // currently only updating player
    auto position = player->physics.position;
    auto velocity = player->physics.velocity;
    auto acceleration = player->physics.acceleration;

    velocity += 0.5f * acceleration * dt;
    position += velocity * dt;
    velocity += 0.5f * acceleration * dt;
    velocity.x *= player->physics.damping;
    velocity.x = std::clamp(velocity.x, -player->physics.terminal_velocity, player->physics.terminal_velocity);
    velocity.y = std::clamp(velocity.y, -player->physics.terminal_velocity, player->physics.terminal_velocity);

    // check for collisions in the world - x direction
    Vec<float> future_position{position.x, player->physics.position.y};
    Vec<float> future_velocity{velocity.x, 0};
    move_to(future_position, player->size, future_velocity);
    // now y direction after (maybe) moving in x
    future_velocity.y = velocity.y;
    future_position.y = position.y;
    move_to(future_position, player->size, future_velocity);
    // update the player position and velocity
    player->physics.velocity = future_velocity;
    player->physics.position = future_position;
}

void World::load_level(const Level &level) {
    for (const auto& [pos, tile_id] : level.tile_locations) {
        tilemap(pos.x, pos.y) = level.tile_types.at(tile_id);
    }
    audio->load_sounds({});
}


