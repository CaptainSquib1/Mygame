#include "world.h"
#include <SDL3/SDL_rect.h>
#include <algorithm>
#include "game_object.h"

#include "vec.h"
#include "physics.h"

World::World(int width, int height)
    : tilemap{width, height}  {

}

void World::add_platform(float x, float y, float width, float height) {
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            tilemap(x+j,y+i) = Tile::Platform;
        }
    }
}

void World::move_to(Vec<float> &position, const Vec<int> &size, Vec<float> &velocity) {
    // test for collisions on the bottom or top first

    Vec<float> topRight = {position.x + size.x, position.y + size.y};
    Vec<float> topLeft = {position.x, position.y + size.y};

    if (collides(topRight) && collides(topLeft)) {
        position.y = floor(position.y);
        velocity.y = 0;
    }


    Vec<float> bottomRight = {position.x + size.x, position.y};
    Vec<float> bottomLeft = position;
    if (collides(bottomRight) && collides(bottomLeft)) {
        position.y = ceil(position.y);
        velocity.y = 0;
    }


    // then test for collisions on the left and right sides


    if (collides(topLeft) && collides(bottomLeft)) {
        position.x = ceil(position.x);
        velocity.x = 0;
    }

    if (collides(topRight) && collides(bottomRight)) {
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
    return tilemap(x,y) == Tile::Platform;
}

GameObject* World::create_player() {
    game_object = std::make_unique<GameObject>(Vec<float>{10, 5}, Vec<int>{1, 1}, *this);

    return game_object.get();
}

void World::update(float dt) {
    // currently only updating player
    auto position = game_object->physics.position;
    auto velocity = game_object->physics.velocity;
    auto acceleration = game_object->physics.acceleration;

    velocity += 0.5f * acceleration * dt;
    position += velocity * dt;
    velocity += 0.5f * acceleration * dt;
    velocity.x *= game_object->physics.damping;
    velocity.x = std::clamp(velocity.x, -game_object->physics.terminal_velocity, game_object->physics.terminal_velocity);
    velocity.y = std::clamp(velocity.y, -game_object->physics.terminal_velocity, game_object->physics.terminal_velocity);

    // check for collisions in the world - x direction
    Vec<float> future_position{position.x, game_object->physics.position.y};
    Vec<float> future_velocity{velocity.x, 0};
    move_to(future_position, game_object->size, future_velocity);
    // now y direction after (maybe) moving in x
    future_velocity.y = velocity.y;
    future_position.y = position.y;
    move_to(future_position, game_object->size, future_velocity);
    // update the player position and velocity
    game_object->physics.velocity = future_velocity;
    game_object->physics.position = future_position;
}

