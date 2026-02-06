#include "player.h"


#include "physics.h"


Player::Player(const Vec<float> &position, const Vec<float> &size)
    : position{position}, size{size}, velocity{0,0}{
    acceleration.y = gravity;
}

void Player::handle_input() {

    const bool *key_states = SDL_GetKeyboardState(NULL);
    // velocity.x = velocity.y = 0;
    acceleration.x = 0;

    // removed for top down controls
    // if (key_states[SDL_SCANCODE_W] || key_states[SDL_SCANCODE_UP]) {
    //     velocity.y += -8;
    // }
    // if (key_states[SDL_SCANCODE_S] || key_states[SDL_SCANCODE_DOWN]) {
    //     velocity.y += +8;
    // }

    if (key_states[SDL_SCANCODE_A] || key_states[SDL_SCANCODE_LEFT]) {
       //velocity.x += -8;
        acceleration.x += -walk_acceleration;
    }
    if (key_states[SDL_SCANCODE_D] || key_states[SDL_SCANCODE_RIGHT]) {
        //velocity.x += +8;
        acceleration.x += walk_acceleration;
    }
    if (key_states[SDL_SCANCODE_SPACE] && velocity.y >= 0) {
        velocity.y = jump_velocity;
    }

}

void Player::update() {}

std::pair<Vec<float>, Color> Player::get_sprite() const {
    return {position, {255, 0, 255, 255}};
}
