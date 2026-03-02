#pragma once

#include "vec.h"
#include "sprite.h"
#include "json.hpp"
#include "graphics.h"

// need to map Vec to JSON
template <typename T>
void to_json(const nlohmann::json& j, const Vec<T>& v) {
    j = nlohmann::json::array({v.x, v.y});
}

template <typename T>
void from_json(const nlohmann::json& j, Vec<T>& v) {
    v.x = j.at(0).get<T>();
    v.y = j.at(1).get<T>();
}

// needed for json conversions
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Sprite, filename, location, size, scale);

namespace AssetManager {
    Sprite get_game_object_sprite(const std::string& name, Graphics& graphics);

}