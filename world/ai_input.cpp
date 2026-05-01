#include "ai_input.h"

#include <iostream>

#include "fsm.h"
#include "game_object.h"
#include "world.h"

void AiInput::get_input() {

}

void AiInput::handle_input(World &world, GameObject &obj) {
    constexpr float epsilon = 1e-4;

    // wall check points
    Vec<float> left   = {obj.physics.position.x - epsilon,obj.physics.position.y};
    Vec<float> right  = {obj.physics.position.x + obj.size.x + epsilon, obj.physics.position.y};
    Vec<float> leftm  = {obj.physics.position.x - epsilon,obj.physics.position.y + obj.size.y / 2};
    Vec<float> rightm = {obj.physics.position.x + obj.size.x + epsilon, obj.physics.position.y + obj.size.y / 2};

    // one tile above
    Vec<float> leftup  = {obj.physics.position.x - epsilon,obj.physics.position.y + obj.size.y + (obj.size.y /2)};
    Vec<float> rightup = {obj.physics.position.x + obj.size.x + epsilon, obj.physics.position.y + obj.size.y + (obj.size.y /2) };

    // foot points for on_ground
    Vec<float> foot_left  = {obj.physics.position.x + epsilon,obj.physics.position.y - epsilon};
    Vec<float> foot_right = {obj.physics.position.x + obj.size.x - epsilon, obj.physics.position.y - epsilon};

    bool on_ground = world.collides(foot_left) || world.collides(foot_right);

    if (on_ground) {
        if (world.collides(left) || world.collides(leftm)) {
            last_move = ActionType::MoveLeft;
            bool jumpable = !world.collides(leftup);
            // std::cout << "leftcheck\n";
            next_action_type = (jumpable) ? ActionType::Jump : ActionType::MoveRight;
        }

        if (world.collides(right) || world.collides(rightm)) {
            last_move = ActionType::MoveRight;
            bool jumpable = !world.collides(rightup);
            // std::cout << "rightcheck\n";
            next_action_type = (jumpable) ? ActionType::Jump : ActionType::MoveLeft;
        }
    }

    if (!on_ground && next_action_type == ActionType::Jump) {
        next_action_type = last_move;
        // std::cout << "not ground\n";
    }

    Action* action = obj.fsm->current_state->input(world, obj, next_action_type);
    if (action) {
        action->perform(world, obj);
        delete action;
    }
}