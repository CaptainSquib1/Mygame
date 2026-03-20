#pragma once

#include <memory>
#include <SDL3/SDL.h>
#include <vector>
#include "tilemap.h"
#include "vec.h"



class GameObject;
class Level;

class World {
public:
    World(const Level& level);

    void add_platform(float x, float y, float width, float height);
    void move_to(Vec<float>& position, const Vec<int>& size, Vec<float>& velocity);
    bool collides(const Vec<float>& position) const;
    GameObject* create_player(const Level& level);
    void update(float dt);
    void load_level(const Level& level);

    Tilemap tilemap;
private:
    GameObject* player;
};
