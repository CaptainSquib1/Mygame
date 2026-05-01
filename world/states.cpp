#include "states.h"
#include "action.h"
#include "game_object.h"
#include "random.h"


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
    if (action_type == ActionType::ThrowKnife) {
        return new ThrowKnife();
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
    if (elapsed <= 0 && (obj.obj_name != "player")){
        auto flip = 0;
        flip = randint(1,100);
        if (flip <= 20) {
            obj.fsm->transition(Transition::Stop, world, obj);
        }
        else {
            obj.fsm->transition(Transition::Swing, world, obj);
        }

    }
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
    if (action_type == ActionType::ThrowKnife) {
        return new ThrowKnife();
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
    if (action_type == ActionType::ThrowKnife) {
        return new ThrowKnife();
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
    // obj.set_sprite("attacking");
    // obj.color ={255,255,255,255};
    // for (auto& enemy: world.game_objects) {
    //     if (enemy == world.player /*|| enemy == world.network_player*/) continue;
    //     enemy->take_damage(obj.damage);
    // }
    // elapsed = 0;
}

void AttackAllEnemies::update(World &world, GameObject &obj, double dt) {
    elapsed += dt;
    if (elapsed >= cooldown) {
        obj.fsm->transition(Transition::Stop, world, obj);
    }
}
void AttackAllEnemies::on_exit(World& , GameObject&) {
}

// Patrolling
void Patrolling::on_enter(World& world, GameObject& obj) {
    // set cooldown to a random amount of time 3-10sec
    elapsed = 0;
    cooldown = randint(4,20);
    Walking::on_enter(world, obj);

}

Action *Patrolling::input(World &world, GameObject &obj, ActionType action_type) {
    if (elapsed >= cooldown) {
        obj.fsm->transition(Transition::Stop, world, obj);
        return nullptr;
    }
    if (action_type == ActionType::Jump) {
        obj.fsm->transition(Transition::Jump, world, obj);
        return new Jump();
    }
    if (action_type == ActionType::MoveLeft) {
        obj.flipped = true;
        return new MoveLeft();
    }
    if (action_type == ActionType::MoveRight) {
        obj.flipped = false;
        return new MoveRight();
    }
    return nullptr;
}

void Patrolling::update(World &, GameObject &, double dt) {
    elapsed += dt;
}

// Watching
// TODO Watching Wrapper for Standing
void Watching::on_enter(World& world, GameObject& obj) {
    // set cooldown to a random amount of time 3-10sec
    elapsed = 0;
    cooldown = randint(2,5);
    Standing::on_enter(world, obj);

}
Action* Watching::input(World &world, GameObject &obj, ActionType action_type) {
    if (elapsed >= cooldown) {
        obj.fsm->transition(Transition::Move, world, obj);
        return nullptr;
    }
    return Standing::input(world, obj, ActionType::None);
}

void Watching::update(World &, GameObject &, double dt) {
    elapsed += dt;
}

// enum class StateType {Standing,InAir,Walking,Dashing,Swinging,Crouching};
// enum class Transition {Jump,Stop,Move,Dash,Swing,Crouch};

