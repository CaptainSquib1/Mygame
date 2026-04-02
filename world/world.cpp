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
#include "events.h"

World::World(const Level& level, Audio& audio, GameObject* player, Events events) : tilemap{level.width, level.height}, audio{&audio}, events{events}, player{player}{
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

    touch_tiles(*player);
}

void World::load_level(const Level &level) {
    for (const auto& [pos, tile_id] : level.tile_locations) {
        tilemap(pos.x, pos.y) = level.tile_types.at(tile_id);
    }
    audio->load_sounds({});

    //get all enemies
    for (const auto& [pos,enemy_name] : level.enemy_locations) {
        GameObject enemy{enemy_name, nullptr, nullptr,{255,0,0,255}};
        enemy.physics.position = pos;
        game_objects.push_back(enemy);
    }
}

void World::touch_tiles(GameObject &obj) {
    int x = std::floor(obj.physics.position.x);
    int y = std::floor(obj.physics.position.y);
    const std::vector<Vec<float>> displacements{{0,0}, {static_cast<float>(obj.size.x), 0}, {0, static_cast<float>(obj.size.y)}, {static_cast<float>(obj.size.x), static_cast<float>(obj.size.y)}};
    for (const auto& displacement : displacements) {
        Tile& tile = tilemap(x + displacement.x, y + displacement.y);
        if (!tile.event_name.empty()) {
            auto itr = events.find(tile.event_name);
            if (itr == events.end()) {
                throw std::runtime_error("Cannot find event: "+ tile.event_name);
            }
            itr->second->perform(*this, obj);
        }
    }
}

