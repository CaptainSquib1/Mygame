#pragma once
#include <SDL3/SDL.h>
#include <utility>
#include "graphics.h"
#include "vec.h"


class Player {
public:
    Player(const Vec<float>& position, const Vec<float>& size);
    void handle_input();
    void update();
    std::pair<Vec<float>, Color> get_sprite() const;

// data
    Vec<float> position;
    Vec<float> size;
    Vec<float> velocity;
    Vec<float> acceleration;
};
