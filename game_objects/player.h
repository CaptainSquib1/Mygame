#pragma once
#include <SDL3/SDL.h>
#include <utility>
#include "graphics.h"


class Player {
public:
    Player(float x, float y, float size);
    void handle_input();
    void update();
    std::pair<SDL_FRect, Color> get_sprite() const;

// data
    SDL_FRect bounding_box;
    float vx, vy;
};