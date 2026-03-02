#include "game_object.h"
#include "action.h"
#include "input.h"

#include "fsm.h"
#include "physics.h"


GameObject::GameObject(const Vec<float> &position, const Vec<int>& size, World& world, FSM* fsm, Input* input, Color color)
    : physics{position, {0,0},{0,0}},size{size}, fsm {fsm}, input{input}, color {color}  {
    physics.acceleration.y = physics.gravity;
    fsm->current_state->on_enter(world, *this);
}

GameObject::~GameObject() {
    delete fsm;
    delete input;
}


void GameObject::update(World& world, double dt) {
    fsm->current_state->update(world, *this, dt);
}

std::pair<Vec<float>, Color> GameObject::get_sprite() const {
    return {physics.position, color};
}
