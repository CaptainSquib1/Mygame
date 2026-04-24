#pragma once
#include "input.h"


class NetworkInput : public Input{
public:
    int create_connection();
    void translate_input();
    void send_game_data(World &world);
    Action* collect_discrete_event(SDL_Event *event) override;
    void get_input() override;
    void handle_input(World &world, GameObject &obj) override;
};
