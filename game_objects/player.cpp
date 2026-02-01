#include "player.h"


Player::Player(float x, float y, float size)
    : bounding_box{x, y, size, size}, vx{0}, vy{0}{}

void Player::handle_input() {

    const bool *key_states = SDL_GetKeyboardState(NULL);
    vx = vy = 0;

    if (key_states[SDL_SCANCODE_W] || key_states[SDL_SCANCODE_UP]) {
        vy += -16;
    }
    if (key_states[SDL_SCANCODE_S] || key_states[SDL_SCANCODE_DOWN]) {
        vy += +16;
    }
    if (key_states[SDL_SCANCODE_A] || key_states[SDL_SCANCODE_LEFT]) {
       vx += -16;
    }
    if (key_states[SDL_SCANCODE_D] || key_states[SDL_SCANCODE_RIGHT]) {
        vx += +16;
    }

}

void Player::update() {}

std::pair<SDL_FRect, Color> Player::get_sprite() const {
    return {bounding_box, {255, 0, 255, 255}};
}
