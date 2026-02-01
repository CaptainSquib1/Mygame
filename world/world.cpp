#include "world.h"
#include <SDL3/SDL_rect.h>
#include <algorithm>
#include "player.h"
#include "vec.h"

void World::add_platform(float x, float y, float width, float height) {
    SDL_FRect rect{x, y, width, height};
    platforms.push_back(rect);
}

const std::vector<SDL_FRect>& World::get_platforms() const {
    return platforms;
}

bool World::has_any_collisions(const SDL_FRect& box) const {
    return std::any_of(std::begin(platforms), std::end(platforms),
        [&](const SDL_FRect& platform){return SDL_HasRectIntersectionFloat(&platform, &box);});
}

Player* World::create_player() {
    player = std::make_unique<Player>(Vec<float>{600, 300}, Vec<float>{16,16});
    return player.get();
}

void World::update() {
    // currently only updating player
    SDL_FRect future{player->position.x, player->position.y, player->size.x,player->size.y};
    future.x += player->velocity.x;
    future.y += player->velocity.y;

    //check for collisions with world
    if (!has_any_collisions(future)) {
        player->position.x = future.x;
        player->position.y = future.y;
    }
}

