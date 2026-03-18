#pragma once

enum class ActionType {
    None,
    Jump,
    MoveLeft,
    MoveRight,
    DashLeft,
    DashRight,
    Crouch,
    SwingLeft,
    SwingRight,
    ParticleActivate,

};

class World;
class GameObject;

class Action {
public:
    virtual ~Action() = default;
    virtual void perform(World& world, GameObject& obj) = 0;
};

class Jump : public Action {
    void perform(World&, GameObject& obj) override;
};

class MoveRight : public Action {
    void perform(World&, GameObject& obj) override;
};
class MoveLeft : public Action {
    void perform(World&, GameObject& obj) override;
};

class DashRight : public Action {
    void perform(World&, GameObject& obj) override;
};
class DashLeft : public Action {
    void perform(World&, GameObject& obj) override;
};

class Crouch : public Action {
    void perform(World&, GameObject& obj) override;
};

class SwingLeft : public Action {
   void perform(World&, GameObject& obj) override;
};
class SwingRight : public Action {
    void perform(World&, GameObject& obj) override;
};

class ParticleActivate : public Action {
    void perform(World&, GameObject& obj) override;
};
