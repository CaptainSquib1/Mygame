#pragma once
#include <map>
#include <string>


class World;
class GameObject;
class Event;

using Events = std::map<std::string, Event*>;

class Event {
public:
    virtual ~Event() = default;
    virtual void perform(World& world, GameObject& obj) = 0;

};

class NextLevel : public Event {
public:
    void perform(World &world, GameObject &obj) override;
};

class PreviousLevel : public Event {
public:
    void perform(World &world, GameObject &obj) override;
};

class TreasureRoom : public Event {
public:
    void perform(World &world, GameObject &obj) override;
};

class Spikes : public Event {
public:
    void perform(World &world, GameObject &obj) override;
};

class Water : public Event {
public:
    void perform(World &world, GameObject &obj) override;
};

class OutTreasureRoom : public Event {
public:
    void perform(World &world, GameObject &obj) override;
};

class Win : public Event {
    public:
    void perform(World &world, GameObject &obj) override;
};