#pragma once
#include "fsm.h"
#include "world.h"

// enum class StateType {Standing,InAir,Walking,Dashing,Swinging,Crouching};
// enum class Transition {Jump,Stop,Move,Dash,Swing,Crouch};

class Standing: public State {
    public:
    void on_enter(World &, GameObject &) override;
    Action* input(World &, GameObject &, ActionType) override;
};

class InAir: public State {
    public:
    void on_enter(World &, GameObject &) override;
    void update(World &, GameObject &, double dt) override;
    Action* input(World &, GameObject &, ActionType) override;

    const double cooldown = 0.1;
    double elapsed = 0;
};

class Walking: public State {
    public:
    void on_enter(World &, GameObject &) override;
    Action* input(World &, GameObject &, ActionType) override;
};

class Swinging: public State {
    public:
    void on_enter(World &, GameObject &) override;
    Action* input(World &, GameObject &, ActionType) override;
    void update(World &, GameObject &, double dt) override;
    const double cooldown = .1;
    double elapsed = 0;

};
class Dashing: public State {
    public:
    void on_enter(World &, GameObject &) override;
    Action* input(World &, GameObject &, ActionType) override;

};

class Crouching: public State {
    public:
    void on_enter(World &, GameObject &) override;
    Action* input(World &, GameObject &, ActionType) override;
    void on_exit(World &, GameObject &) override;

};