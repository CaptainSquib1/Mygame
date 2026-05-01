#pragma once
#include "input.h"



class AiInput : public Input{
    public:
    void get_input() override;
    void handle_input(World &world, GameObject &obj) override;
    ActionType last_move = ActionType::None;

};
