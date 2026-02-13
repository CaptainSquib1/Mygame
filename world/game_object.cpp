#include "game_object.h"


#include "physics.h"


GameObject::GameObject(const Vec<float> &position, const Vec<int>& size, World& world)
    : size{size} {
    physics.position = position;
    physics.acceleration.y = physics.gravity;
}

GameObject::~GameObject() {
}

void GameObject::input(World& world) {

    const bool *key_states = SDL_GetKeyboardState(NULL);
    // velocity.x = velocity.y = 0;
    physics.acceleration.x = 0;

    // removed top down controls
    // if (key_states[SDL_SCANCODE_W] || key_states[SDL_SCANCODE_UP]) {
    //     velocity.y += -8;
    // }
    // if (key_states[SDL_SCANCODE_S] || key_states[SDL_SCANCODE_DOWN]) {
    //     velocity.y += +8;
    // }

    if (key_states[SDL_SCANCODE_A] || key_states[SDL_SCANCODE_LEFT]) {
        //velocity.x += -8;
        physics.acceleration.x += -physics.walk_acceleration;
    }
    if (key_states[SDL_SCANCODE_D] || key_states[SDL_SCANCODE_RIGHT]) {
        //velocity.x += +8;
        physics.acceleration.x += physics.walk_acceleration;
    }
    if (key_states[SDL_SCANCODE_SPACE]) {
        physics.velocity.y = physics.jump_velocity;
    }

}

void GameObject::update(World& world, double dt) {}

std::pair<Vec<float>, Color> GameObject::get_sprite() const {
    return {physics.position, {255, 0, 255, 255}};
}
