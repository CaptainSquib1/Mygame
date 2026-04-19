#include "states.h"
#include "action.h"
#include "game_object.h"



// helper function
bool on_platform(World& world, GameObject& obj) {
    constexpr float epsilon = 1e-4;
    Vec<float> left_foot{obj.physics.position.x + epsilon, obj.physics.position.y - epsilon};
    Vec<float> right_foot{obj.physics.position.x + obj.size.x - epsilon, obj.physics.position.y - epsilon};
    return world.collides(left_foot) || world.collides(right_foot);
}


//////////////
///Standing
//////////////
void Standing::on_enter(World &, GameObject & obj) {
    obj.color = {255,0,0,255};
    obj.set_sprite("idle");
    obj.physics.acceleration.x = 0;
}

Action *Standing::input(World& world, GameObject& obj, ActionType action_type) {
    if (action_type == ActionType::AttackAll) {
        obj.fsm->transition(Transition::AttackAll, world,obj);
    }
    if (action_type == ActionType::Jump) {
        obj.fsm->transition(Transition::Jump, world,obj);
        return new Jump();
    }
    if (action_type == ActionType::Crouch) {
        obj.fsm->transition(Transition::Crouch, world, obj);
        return new Crouch();
    }
    if (action_type == ActionType::MoveLeft) {
        obj.flipped = true;
        obj.fsm->transition(Transition::Move,world,obj);
        return new MoveLeft();
    }
    if (action_type == ActionType::MoveRight) {
        obj.flipped = false;
        obj.fsm->transition(Transition::Move,world,obj);
        return new MoveRight();
    }
    if (action_type == ActionType::DashLeft) {
        obj.fsm->transition(Transition::Dash,world,obj);
        return new DashLeft();
    }
    if (action_type == ActionType::DashRight) {
        obj.fsm->transition(Transition::Dash,world,obj);
        return new DashRight();
    }
    return nullptr;
}
// In Air
void InAir::on_enter(World &world, GameObject &obj) {
    elapsed = cooldown;
    obj.set_sprite("jumping");
    obj.color = {0,0,255,255};
}

void InAir::update(World &world, GameObject &obj, double dt) {
    elapsed -= dt;
    if (elapsed <= 0 && on_platform(world, obj)) {
        obj.fsm->transition(Transition::Stop, world, obj);
    }
}

Action *InAir::input(World &world, GameObject &obj, ActionType action_type) {
    if (action_type == ActionType::None) {
        return nullptr;
    }
    if (action_type == ActionType::MoveLeft) {
        obj.flipped = true;
        obj.fsm->transition(Transition::Move,world,obj);
        return new MoveLeft();
    }
    if (action_type == ActionType::MoveRight) {
        obj.flipped = false;
        obj.fsm->transition(Transition::Move,world,obj);
        return new MoveRight();
    }
    if (action_type == ActionType::DashLeft) {
        obj.fsm->transition(Transition::Dash,world,obj);
        return new DashLeft();
    }
    if (action_type == ActionType::DashRight) {
        obj.fsm->transition(Transition::Dash,world,obj);
        return new DashRight();
    }
    if (action_type == ActionType::SwingLeft) {
        obj.fsm->transition(Transition::Swing,world,obj);
        return new SwingLeft();
    }
    if (action_type == ActionType::SwingRight) {
        obj.fsm->transition(Transition::Swing,world,obj);
        return new SwingRight();
    }
    return nullptr;
}
// Walking
void Walking::on_enter(World &, GameObject & obj) {
    obj.set_sprite("walking");
    obj.color = {0,255,0,255};
}

Action* Walking::input(World& world, GameObject& obj, ActionType action_type) {
    if (action_type == ActionType::None) {
        obj.fsm->transition(Transition::Stop, world, obj);
    }
    if (action_type == ActionType::Jump) {
        obj.fsm->transition(Transition::Jump, world, obj);
        return new Jump();
    }
    if (action_type == ActionType::Crouch) {
        obj.fsm->transition(Transition::Crouch, world, obj);
        return new Crouch();
    }
    if (action_type == ActionType::DashLeft) {
        obj.fsm->transition(Transition::Dash,world,obj);
        return new DashLeft();
    }
    if (action_type == ActionType::DashRight) {
        obj.fsm->transition(Transition::Dash,world,obj);
        return new DashRight();
    }
    return nullptr;
}
// Dashing
void Dashing::on_enter(World &world, GameObject &obj) {
    obj.set_sprite("dashing");
    obj.color = {255,0, 255, 255};
}

Action * Dashing::input(World &world, GameObject &obj, ActionType action_type) {
    if (action_type == ActionType::None) {
        obj.fsm->transition(Transition::Stop, world, obj);
    }
    return nullptr;

}

// Swinging
void Swinging::on_enter(World &, GameObject &obj) {
    elapsed = cooldown;
    obj.set_sprite("swinging");
    obj.color ={200, 200, 0, 255};
}

void Swinging::update(World &world, GameObject &obj, double dt) {
    elapsed -= dt;
    if (elapsed <= 0 && on_platform(world, obj)) {
        obj.fsm->transition(Transition::Stop, world, obj);
    }
}

Action *Swinging::input(World &world, GameObject &obj, ActionType action_type) {

    if (action_type == ActionType::None && on_platform(world,obj)) {
        obj.fsm->transition(Transition::Stop, world, obj);
    }
    if (action_type == ActionType::DashLeft) {
        obj.fsm->transition(Transition::Dash,world,obj);
    }
    if (action_type == ActionType::DashRight) {
        obj.fsm->transition(Transition::Dash,world,obj);
    }
    return nullptr;
}


//Crouching
void Crouching::on_enter(World &, GameObject & obj) {
    obj.set_sprite("crouching");
    obj.color ={200,200,200};
}
Action *Crouching::input(World &world, GameObject &obj, ActionType action_type) {
    if (action_type == ActionType::Crouch) {
        obj.fsm->transition(Transition::Stop, world, obj);
    }
    if (action_type == ActionType::MoveLeft) {
        obj.flipped = true;
        obj.fsm->transition(Transition::Move, world, obj);
    }
    if (action_type == ActionType::MoveRight) {
        obj.flipped = false;
        obj.fsm->transition(Transition::Move, world, obj);
    }
    if (action_type == ActionType::Jump) {
        obj.fsm->transition(Transition::Stop, world, obj);
    }
    return nullptr;

}

void Crouching::on_exit(World &, GameObject &obj) {

}
//Attack All
void AttackAllEnemies::on_enter(World &world, GameObject &obj) {
    obj.set_sprite("attacking");
    obj.color ={255,255,255,255};
    for (auto& enemy: world.game_objects) {
        if (enemy == world.player) continue;
        enemy->take_damage(obj.damage);
    }
    elapsed = 0;
}

void AttackAllEnemies::update(World &world, GameObject &obj, double dt) {
    elapsed += dt;
    if (elapsed >= cooldown) {
        obj.fsm->transition(Transition::Stop, world, obj);
    }
}
void AttackAllEnemies::on_exit(World &, GameObject &obj) {
}


// enum class StateType {Standing,InAir,Walking,Dashing,Swinging,Crouching};
// enum class Transition {Jump,Stop,Move,Dash,Swing,Crouch};

