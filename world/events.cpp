#include "events.h"

#include <iostream>

#include "world.h"
#include "game_object.h"

void NextLevel::perform(World &world, GameObject &obj) {
    if (&obj != world.player && &obj != world.network_player) return;
    world.end_level = true;
    world.audio->play_sound("power");
}

void PreviousLevel::perform(World &world, GameObject& obj) {
    if (&obj != world.player && &obj != world.network_player) return;
    world.back_level = true;
    world.audio->play_sound("power");
}

void TreasureRoom::perform(World &world, GameObject &obj) {
    if (&obj != world.player && &obj != world.network_player) return;
    world.treasure_level = true;
    world.audio->play_sound("coin");
}
void OutTreasureRoom::perform(World &world, GameObject &obj) {
    if (&obj != world.player && &obj != world.network_player) return;
    world.end_treasure_level = true;
    world.audio->play_sound("coin");
}

void Spikes::perform(World &world, GameObject &obj) {
    obj.take_damage(20);
    world.audio->play_sound("hurt");
}

void Water::perform(World &world, GameObject &obj) {
    std::cout << "water\n";
}
void Win::perform(World &world, GameObject &) {
    world.audio->play_sound("coin");
    world.win_game = true;
}
