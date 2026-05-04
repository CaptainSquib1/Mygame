#include "world.h"
#include <SDL3/SDL_rect.h>
#include <algorithm>
#include <iostream>

#include "game_object.h"
#include "fsm.h"
#include "states.h"
#include "level.h"
#include "vec.h"
#include "physics.h"
#include "keyboard_input.h"
#include "audio.h"
#include "events.h"

World::World(const Level& level, Audio& audio, GameObject* player, GameObject* network_player, Events events)
    : tilemap{level.width, level.height}, audio{&audio},
        events{events}, player{player}, network_player{network_player},
        quadtree{AABB{{level.width / 2.0f, level.height/2.0f},
            {level.width / 2.0f, level.height/2.0f}}}
{
    load_level(level);
}

World::~World() {
    for (auto obj : game_objects) {
        if (obj == player || obj == network_player) continue;
        delete obj;
    }
}

void World::add_platform(float x, float y, float width, float height) {
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            tilemap(x+j,y+i) = Tile{};
        }
    }
}

void World::move_to(Vec<float> &position, const Vec<float> &size, Vec<float> &velocity) {
    // test for collisions on the bottom or top first

    Vec<float> topRight = {position.x + size.x, position.y + size.y};
    Vec<float> topLeft = {position.x, position.y + size.y};

    Vec<float> topmiddle = {topLeft.x + .5f, topLeft.y};
    if ((collides(topRight) && collides(topLeft)) || collides(topmiddle)) {
        position.y = floor(position.y);
        velocity.y = 0;
    }


    Vec<float> bottomRight = {position.x + size.x, position.y};
    Vec<float> bottomLeft = position;
    Vec<float> bottommiddle = {bottomLeft.x + .5f, bottomLeft.y};
    if ((collides(bottomRight) && collides(bottomLeft)) || collides(bottommiddle)) {
        position.y = ceil(position.y);
        velocity.y = 0;
    }


    // then test for collisions on the left and right sides

    Vec<float> leftmiddle = {bottomLeft.x,bottomLeft.y + .5f};
    if ((collides(topLeft) && collides(bottomLeft)) || collides(leftmiddle)) {
        position.x = ceil(position.x);
        velocity.x = 0;
    }
    Vec<float> rightmiddle = {bottomRight.x, bottomRight.y + .5f};
    if ((collides(topRight) && collides(bottomRight)) || collides(rightmiddle)) {
        position.x = floor(position.x);
        velocity.x = 0;
    }

    // now test each corner
    if (collides(bottomLeft)) {
        auto dx = ceil(position.x) - position.x;
        auto dy = ceil(position.y) - position.y;
        if (dx > dy) {
            position.y = ceil(position.y);
            velocity.y = 0;
        }
        else {
            position.x = ceil(position.x);
            velocity.x = 0;
        }
    }
    if (collides(topLeft)) {
        auto dx = ceil(position.x) - position.x;
        auto dy = floor(position.y + size.y) - (position.y + size.y);
        if (std::abs(dx) > std::abs(dy)) {
            position.y = dy + position.y;
            velocity.y = 0;
        }
        else {
            position.x = dx + position.x;
            velocity.x = 0;
        }
    }
    if (collides(topRight)) {
        auto dx = floor(position.x + size.x) - (position.x + size.x);
        auto dy = floor(position.y + size.y) - (position.y + size.y);
        if (dx < dy) {
            position.y = dy + position.y;
            velocity.y = 0;
        }
        else {
            position.x = dx + position.x;
            velocity.x = 0;
        }
    }

    if (collides(bottomRight)) {
        auto dx = floor(position.x + size.x) - (position.x + size.x);
        auto dy = ceil(position.y) - (position.y);
        if (std::abs(dx) > std::abs(dy)) {
            position.y = position.y + dy;
            velocity.y = 0;
        }
        else {
            position.x = position.x + dx;
            velocity.x = 0;
        }
    }
}

bool World::collides(const Vec<float>& position) const {
    int x = std::floor(position.x);
    int y = std::floor(position.y);
    return tilemap(x,y).blocking;
}


void World::update(double dt) {
    for (auto& obj : game_objects) {

        update_object(obj, dt);

        touch_tiles(*obj);

    }

    for (auto& tile : tilemap.tiles) {
        tile.update(dt);
    }

    //update projectiles
    for (auto& projectile : projectiles) {
        update_object(projectile, dt);
    }

    // check for collision with the player
    build_quadtree();
    std::vector<GameObject*> collide_with = quadtree.query_range(player->get_bounding_box());
    for (auto& obj : collide_with) {
        if (obj == player || obj == network_player) continue;
        if (player->invincible_time_remaining <= 0) {
            player->take_damage(obj->damage);
            audio->play_sound("hurt");
        }
    }

    // check for collision with projectile and enemy
    for (auto& projectile: projectiles) {
        std::vector<GameObject*> p_collides_with = quadtree.query_range(projectile->get_bounding_box());
        for (auto& obj : p_collides_with) {
            if (obj == player || obj == network_player) continue;

            obj->take_damage(projectile->damage);
            audio->play_sound("hurt");
            projectile->elapsed += projectile->lifetime;
        }
    }

    // std::partition puts objects that return TRUE at the beginning.
    // So we flip the logic: Keep alive objects at the front.
    auto itr = std::stable_partition(game_objects.begin(), game_objects.end(),
        [](GameObject* obj) { return obj->is_alive; }
    );
    game_objects.erase(itr, game_objects.end());
    // Now [itr, end) contains the original pointers to the dead objects
    std::for_each(itr, game_objects.end(), [](GameObject* p) {delete p;});
    game_objects.erase(itr, game_objects.end());

    // same for projectiles
    auto p_itr = std::stable_partition(projectiles.begin(), projectiles.end(),
        [](Projectile* projectile) { return projectile->elapsed <= projectile->lifetime; }   );

    std::for_each(p_itr, projectiles.end(), [](Projectile* p) { delete p; });
    projectiles.erase(p_itr, projectiles.end());


    // check for player death
    if (!player->is_alive || !network_player->is_alive) {
        audio->play_sound("explosion");
        end_game = true;
        return;
    }
}

void World::load_level(const Level &level) {
    for (const auto& [pos, tile_id] : level.tile_locations) {
        tilemap(pos.x, pos.y) = level.tile_types.at(tile_id);
    }
    audio->load_sounds(level.sounds);

    // get the backgrounds
    backgrounds = level.backgrounds;


    //get all enemies
    for (const auto& [pos,enemy_name] : level.enemy_locations) {
        auto enemy = new GameObject{enemy_name, nullptr, nullptr,{255,0,0,255}};
        enemy->physics.position = pos;
        game_objects.push_back(enemy);
    }
    game_objects.push_back(player);
    game_objects.push_back(network_player);
}

void World::touch_tiles(GameObject &obj) {
    float x = obj.physics.position.x;
    float y = obj.physics.position.y;
    float e = -.00001;
    const std::vector<Vec<float>> displacements{{e,e}, //leftb
        {(obj.size.x), e}, //rightb
        {e, obj.size.y+e}, //leftt
        {(obj.size.x), (obj.size.y)}, //rightt
        {e, (obj.size.y)/2}, //leftm
        {(obj.size.x), (obj.size.y)/2}, //rightm
        {(obj.size.x)/2, (obj.size.y)}, //topm
        {(obj.size.x)/2, e},//btmm
    };
    for (const auto& displacement : displacements) {
        Tile& tile = tilemap(x + displacement.x, y + displacement.y);
        if (!tile.event_name.empty()) {
            auto itr = events.find(tile.event_name);
            if (itr == events.end()) {
                throw std::runtime_error("Cannot find event: "+ tile.event_name);
            }
            itr->second->perform(*this, obj);
        }
    }
}

void World::build_quadtree() {
    quadtree.clear();
    for (auto obj : game_objects) {
        quadtree.insert(obj);
    }
}

void World::update_object(GameObject* obj, double dt) {
    // updating obj
    obj->update(*this,dt);
    auto position = obj->physics.position;
    auto velocity = obj->physics.velocity;
    auto acceleration = obj->physics.acceleration;

    velocity += 0.5f * acceleration * static_cast<float>(dt);
    position += velocity * static_cast<float>(dt);
    velocity += 0.5f * acceleration * static_cast<float>(dt);
    velocity.x *= obj->physics.damping;
    velocity.x = std::clamp(velocity.x, -obj->physics.terminal_velocity, obj->physics.terminal_velocity);
    velocity.y = std::clamp(velocity.y, -obj->physics.terminal_velocity, obj->physics.terminal_velocity);

    // check for collisions in the world - x direction
    Vec<float> future_position{position.x, obj->physics.position.y};
    Vec<float> future_velocity{velocity.x, 0};
    move_to(future_position, obj->size, future_velocity);
    // now y direction after (maybe) moving in x
    future_velocity.y = velocity.y;
    future_position.y = position.y;
    move_to(future_position, obj->size, future_velocity);
    // update the player position and velocity
    obj->physics.velocity = future_velocity;
    obj->physics.position = future_position;
}

