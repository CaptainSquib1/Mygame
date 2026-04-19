#include "NetworkInput.h"
#include "game_object.h"
#include <sys/socket.h>
#include "json.hpp"
#include <arpa/inet.h>
#include <filesystem>
#include <fstream>

#include "world.h"
using json = nlohmann::json;



#include "fsm.h"

void NetworkInput::get_input() {

    // if (next_action_type == ActionType::Jump) return;
    // const bool *key_states = SDL_GetKeyboardState(NULL);

    // if (left) {
    //     if (key_states[SDL_SCANCODE_LALT]) {
    //         next_action_type = ActionType::DashLeft;
    //     }
    //     else if (key_states[SDL_SCANCODE_LSHIFT]) {
    //         next_action_type = ActionType::SwingLeft;
    //     }
    //     else {
    //         next_action_type = ActionType::MoveLeft;
    //     }
    // }
    // if (key_states[SDL_SCANCODE_D] || key_states[SDL_SCANCODE_RIGHT]) {
    //     if (key_states[SDL_SCANCODE_LALT]) {
    //         next_action_type = ActionType::DashRight;
    //     }
    //     else if (key_states[SDL_SCANCODE_LSHIFT]) {
    //         next_action_type = ActionType::SwingRight;
    //     }
    //     else {
    //         next_action_type = ActionType::MoveRight;
    //     }
    //
    // }

}

void NetworkInput::handle_input(World &world, GameObject &obj) {

    // Action* action = obj.fsm->current_state->input(world, obj, next_action_type);
    //
    // // consume action
    // next_action_type = ActionType::None;
    // if (action != nullptr) {
    //     action->perform(world, obj);
    //     delete action;
    // }

}

int NetworkInput::create_connection() {
    std::string ip = "localhost"; //local Host for testing TODO: change to ip
    int port = 5005;
    auto  s = socket(AF_INET, SOCK_STREAM, 0);
    if (s == -1) {
        printf("Cannot create socket\n");
        return 1;
    }
    int opt = 1;
    setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt));

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = inet_addr(ip.c_str());

    if (bind(s, (struct sockaddr*)&server, sizeof(server)) < 0) {
        printf("Cannot setup server\n");
        return 1;
    }

    listen(s,1);

    struct sockaddr_in client;
    socklen_t client_len = sizeof(client);
    auto c = accept(s, (struct sockaddr*)&client, &client_len);
    char buffer[4096];

    if (recv(c, buffer, sizeof(buffer), 0)) {
        printf("Connection received from client: %s\n", buffer);
        auto message = "message received";
        send(c, message, strlen(message), 0);

    }

}

void NetworkInput::translate_input() {

}

void NetworkInput::send_game_data(World &world) {
    auto path_start = std::filesystem::current_path() / "communication";
    auto path = path_start/ ("outbound.json");

    std::ifstream file(path);
    if (!file) {
        throw std::runtime_error("Could not open file: " + path.string());
    }

    nlohmann::json json;
    file >> json;
    for (auto objects : world.game_objects) {
        // json.get<>();


    }
}

void NetworkInput::collect_discrete_event(SDL_Event *event) {

    if (event->type == SDL_EVENT_KEY_DOWN && event->key.repeat == 0) {
        if (event->key.scancode == SDL_SCANCODE_SPACE) {
            next_action_type = ActionType::Jump;
        }
        if (event->key.scancode == SDL_SCANCODE_LCTRL) {
            next_action_type = ActionType::Crouch;
        }
    }
}

