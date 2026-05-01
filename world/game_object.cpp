#include "game_object.h"

#include <iostream>

#include "action.h"
#include "input.h"

#include "fsm.h"
#include "physics.h"
#include "quadtree.h"


GameObject::GameObject(std::string name, FSM* fsm, Input* input, Color color)
    : obj_name{name}, fsm {fsm}, input{input}, color {color}  {
    physics.acceleration.y = physics.gravity;

}

GameObject::~GameObject() {
    // std::cout << "GameObject::~GameObject()\n";
    delete fsm;
    delete input;
}


void GameObject::update(World& world, double dt) {
    if (fsm != nullptr) {
        fsm->current_state->update(world, *this, dt);
    }
    sprites[sprite_name].update(dt);
    sprites[sprite_name].flip(physics.acceleration.x < 0 || flipped);
    set_sprite(sprite_name);
    if (invincible_time_remaining > 0.0) invincible_time_remaining -= dt;
}

std::pair<Vec<float>, Color> GameObject::get_sprite() const {
    return {physics.position, color};
}

void GameObject::set_sprite(const std::string &next_sprite) {
    // if the next sprite is a new sprite, reset current sprite
    if (next_sprite!=sprite_name) {
        sprites[sprite_name].reset();

        auto itr = sprites.find(next_sprite);
        if (itr!=sprites.end()) {
            sprite_name = next_sprite;
        }
        else {
            sprite_name = "idle"; // Assume every GameObject has an idle sprite
        }
    }

    sprite = sprites[sprite_name].get_sprite();
}


AABB GameObject::get_bounding_box(){
    Vec<float> half_size = {size.x / 2.0f, size.y / 2.0f};
    Vec<float> center = {physics.position.x + half_size.x, physics.position.y + half_size.y};
    AABB bounding_box = {center, half_size};
    return bounding_box;
}

void GameObject::take_damage(int attack_damage) {
    if (invincible_time_remaining > 0.0) return;

    health -= attack_damage;
    invincible_time_remaining = 2;
    if (health <= 0) {
        is_alive = false;
    }
}

bool GameObject::flash_sprite() const {
    if (invincible_time_remaining <= 0.0) {
        return false;
    }

    // alternate overlay on/off every 80 ms
    return ((SDL_GetTicks() / 80) % 2) == 0;
}
