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
class AABB;

using Sprites = std::pmr::map<std::string, AnimatedSprite>;


class GameObject {
public:
    GameObject(std::string name, FSM* fsm, Input* input, Color color={0,0,0,1});
    ~GameObject();

    virtual void update(World& world, double dt);
    std::pair<Vec<float>, Color> get_sprite() const;
    void set_sprite(const std::string& next_sprite);

    void take_damage(int attack_damage);
    bool flash_sprite() const;

    AABB get_bounding_box();

    // Game_Object data
    std::string obj_name;
    Vec<float> size;
    Physics physics;
    FSM* fsm;
    Input* input;
    Color color;
    Sprites sprites;
    Sprite sprite;
    std::string sprite_name;

    bool flipped = false;
    bool is_alive = true;


    //combat stuff
    int health;
    int max_health;
    int damage;

    double invincible_time_remaining{0.0};
};