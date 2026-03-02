#include "keyboard_input.h"
#include "game_object.h"
#include "fsm.h"

void KeyboardInput::get_input() {
    if (next_action_type == ActionType::Jump) return;
    const bool *key_states = SDL_GetKeyboardState(NULL);

    if (key_states[SDL_SCANCODE_A] || key_states[SDL_SCANCODE_LEFT]) {
        if (key_states[SDL_SCANCODE_LALT]) {
            next_action_type = ActionType::DashLeft;
        }
        if (key_states[SDL_SCANCODE_LSHIFT]) {
            next_action_type = ActionType::SwingLeft;
        }
        else {
            next_action_type = ActionType::MoveLeft;
        }

    }
    if (key_states[SDL_SCANCODE_D] || key_states[SDL_SCANCODE_RIGHT]) {
        if (key_states[SDL_SCANCODE_LALT]) {
            next_action_type = ActionType::DashRight;
        }
        if (key_states[SDL_SCANCODE_LSHIFT]) {
            next_action_type = ActionType::SwingRight;
        }
        else {
            next_action_type = ActionType::MoveRight;
        }

    }

}

void KeyboardInput::handle_input(World &world, GameObject &obj) {
    Action* action = obj.fsm->current_state->input(world, obj, next_action_type);

    // consume action
    next_action_type = ActionType::None;
    if (action != nullptr) {
        action->perform(world, obj);
        delete action;
    }
}

void KeyboardInput::collect_discrete_event(SDL_Event *event) {
    if (event->type == SDL_EVENT_KEY_DOWN && event->key.repeat == 0) {
        if (event->key.scancode == SDL_SCANCODE_SPACE) {
            next_action_type = ActionType::Jump;
        }
    }
}
