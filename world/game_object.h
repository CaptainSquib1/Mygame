#pragma once
#include <map>
#include <utility>

#include "animated_sprite.h"
#include "vec.h"
#include "graphics.h"
#include "physics.h"



class World;
class FSM;
class Input;

using Sprites = std::pmr::map<std::string, AnimatedSprite>;


class GameObject {
public:
    GameObject(const Vec<float> &position, const Vec<int>& size, World& world, FSM* fsm, Input* input, Color color);
    ~GameObject();

    void update(World& world, double dt);
    std::pair<Vec<float>, Color> get_sprite() const;
    void set_sprite(const std::string& next_sprite);

    // Game_Object data
    Vec<int> size;
    Physics physics;
    FSM* fsm;
    Input* input;
    Color color;
    Sprites sprites;
    Sprite sprite;
    std::string sprite_name;
    bool flipped;
};