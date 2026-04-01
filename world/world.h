#pragma once

#include <map>
#include <memory>
#include <SDL3/SDL.h>
#include <vector>

#include "audio.h"
#include "events.h"
#include "tilemap.h"
#include "vec.h"



class GameObject;
class Level;
class Audio;
class Event;

class World {
public:
    World(const Level& level, Audio& audio, GameObject* player, Events events);

    void add_platform(float x, float y, float width, float height);
    void move_to(Vec<float>& position, const Vec<int>& size, Vec<float>& velocity);
    bool collides(const Vec<float>& position) const;
    void update(float dt);
    void load_level(const Level& level);

    Tilemap tilemap;
    bool end_level{false};

private:
    GameObject* player;
    Audio* audio;
    Events events;

    void touch_tiles(GameObject& obj);
};
