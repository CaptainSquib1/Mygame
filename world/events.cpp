#include "events.h"

#include <iostream>

#include "world.h"
#include "game_object.h"

void NextLevel::perform(World &world, GameObject &obj) {
    world.end_level = true;
}

void PreviousLevel::perform(World &world, GameObject& obj) {
    world.back_level = true;
}

void TreasureRoom::perform(World &world, GameObject &obj) {
    std::cout << "treasureRoom\n";
}

void Spikes::perform(World &world, GameObject &obj) {
    std::cout << "spiked\n";
}
