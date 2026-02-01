#pragma once

#include <memory>
#include <SDL3/SDL.h>
#include <vector>

class Player;

class World {
public:
    void add_platform(float x, float y, float width, float height);
    const std::vector<SDL_FRect>& get_platforms() const;
    bool has_any_collisions(const SDL_FRect& box) const;
    Player* create_player();
    void update();

private:
    std::vector<SDL_FRect> platforms;
    std::unique_ptr<Player> player;
};
