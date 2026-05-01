#pragma once

#include <map>
#include <memory>
#include <SDL3/SDL.h>
#include <vector>

#include "audio.h"
#include "events.h"
#include "projectile.h"
#include "quadtree.h"
#include "tilemap.h"
#include "vec.h"



class GameObject;
class Level;
class Audio;
class Event;

class World {
public:
    World(const Level& level, Audio& audio, GameObject* player, GameObject* network_player, Events events);
    ~World();
    void add_platform(float x, float y, float width, float height);
    void move_to(Vec<float>& position, const Vec<float>& size, Vec<float>& velocity);
    bool collides(const Vec<float>& position) const;
    void update(double dt);
    void update_object(GameObject* obj, double dt);
    void load_level(const Level& level);

    Tilemap tilemap;
    bool end_level{false};
    bool back_level{false};
    bool treasure_level{false};
    bool end_treasure_level{false};

    bool end_game{false};
    bool win_game{false};


    GameObject* player;
    GameObject* network_player;
    Audio* audio;
    Events events;
    std::vector<GameObject*> game_objects;
    std::vector<Projectile*> projectiles;

    void touch_tiles(GameObject& obj);
    void build_quadtree();

    std::vector<Background> backgrounds;

    QuadTree quadtree;

    // items
    std::map<std::string, std::function<GameObject*()>> available_items;
};
