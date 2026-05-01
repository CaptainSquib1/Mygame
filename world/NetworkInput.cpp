#include "NetworkInput.h"

#include <cstring>
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "json.hpp"

#include "action.h"
#include "fsm.h"
#include "game_object.h"
#include "world.h"
#include <mutex>


void NetworkInput::start_server() {
    server_socket = socket(AF_INET, SOCK_DGRAM, 0); // SOCK_DGRAM for UDP
    int opt = 1;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt));

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(5005);
    server.sin_addr.s_addr = INADDR_ANY;

    bind(server_socket, (struct sockaddr*)&server, sizeof(server));

    listener_thread = std::thread([this]() { listen_loop(); });
    listener_thread.detach();
}

void NetworkInput::listen_loop() {
    std::mutex mtx;
    char buffer[4096];
    socklen_t client_len = sizeof(client_addr);

    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int bytes = recvfrom(server_socket, buffer, sizeof(buffer), 0,
                             (struct sockaddr*)&client_addr, &client_len);
        if (bytes <= 0) continue;


        try {
            auto action = bytes;
            // auto j = nlohmann::json::parse(buffer);
            // std::string action = j["action"];
            // std::cout << action<< "\n";
            if (action == 1000) {
                std::lock_guard<std::mutex> lock(mtx);
                pending_action = ActionType::MoveLeft;
            }
            else if (action == 1111) {
                std::lock_guard<std::mutex> lock(mtx);
                pending_action = ActionType::ThrowKnife;
            }
            else if (action == 0001) {
                std::lock_guard<std::mutex> lock(mtx);
                pending_action = ActionType::MoveRight;
            }
            else if (action == 0100) {
                std::lock_guard<std::mutex> lock(mtx);
                pending_action = ActionType::Jump;
            }
            else if (action == 0010) {
                std::lock_guard<std::mutex> lock(mtx);
                pending_action = ActionType::Crouch;
            }
        } catch (std::exception& e) {
            std::cout << "Exception: " << e.what() << "\n";
        }
    }
}

void NetworkInput::get_input() {
    next_action_type = pending_action;
}

void NetworkInput::handle_input(World& world, GameObject& obj) {
    Action* action = obj.fsm->current_state->input(world, obj, next_action_type);
    pending_action = ActionType::None;

    if (action) {
        action->perform(world, obj);
        delete action;
    }
}