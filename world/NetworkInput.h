#pragma once
#include <thread>
#include <atomic>
#include <netinet/in.h>
#include "input.h"
#include "action.h"

class NetworkInput : public Input {
public:
    void get_input() override;
    void handle_input(World& world, GameObject& obj) override;
    void start_server();

    void listen_loop();
    ActionType pending_action;
    std::thread listener_thread;
    int server_socket = -1;
    struct sockaddr_in client_addr{};
    bool client_known = false;
};