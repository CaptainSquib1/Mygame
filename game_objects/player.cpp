#include "player.h"


Player::Player(const Vec<float> &position, const Vec<float> &size)
    : position{position}, size{size}, velocity{0,0}{

}

void Player::handle_input() {

    const bool *key_states = SDL_GetKeyboardState(NULL);
    velocity.x = velocity.y = 0;

    if (key_states[SDL_SCANCODE_W] || key_states[SDL_SCANCODE_UP]) {
        velocity.y += -8;
    }
    if (key_states[SDL_SCANCODE_S] || key_states[SDL_SCANCODE_DOWN]) {
        velocity.y += +8;
    }
    if (key_states[SDL_SCANCODE_A] || key_states[SDL_SCANCODE_LEFT]) {
       velocity.x += -8;
    }
    if (key_states[SDL_SCANCODE_D] || key_states[SDL_SCANCODE_RIGHT]) {
        velocity.x += +8;
    }

}

void Player::update() {}

std::pair<SDL_FRect, Color> Player::get_sprite() const {
    return {{position.x,position.y,size.x,size.y}, {255, 0, 255, 255}};
}
