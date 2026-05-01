#pragma once
#include <map>
#include <vector>

#include "animated_sprite.h"
#include "graphics.h"
#include "sprite.h"
using Sprites = std::pmr::map<std::string, AnimatedSprite>;

struct Background {
    std::string filename;
    float scale;
    float distance;
    Sprite sprite;
};

class Tile {
public:
    Sprites sprites;
    Sprite sprite;
    std::string sprite_name;
    bool blocking{false};
    std::string id;
    std::string event_name;
    void set_sprite(const std::string& next_sprite);
    void update(double dt);
};

class Tilemap {
public:
    Tilemap(int width, int height);

    // access tiles
    const Tile& operator()(int x, int y) const;
    Tile& operator()(int x, int y);

    const int width;
    const int height;

    std::vector<Tile> tiles;
private:
    void check_bounds(int x, int y) const;
};