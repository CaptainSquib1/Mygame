#pragma once

#include <memory>
#include <SDL3/SDL.h>
#include <vector>
#include "tilemap.h"
#include "vec.h"


class GameObject;

class World {
public:
    World(int width, int height);

    void add_platform(float x, float y, float width, float height);
    void move_to(Vec<float>& position, const Vec<int>& size, Vec<float>& velocity);
    bool collides(const Vec<float>& position) const;
    GameObject* create_player();
    void update(float dt);

    Tilemap tilemap;
private:
    std::unique_ptr<GameObject> game_object;
};
