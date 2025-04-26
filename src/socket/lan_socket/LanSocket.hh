#pragma once

#include "ASocket.hh"
#include "socket/wan_socket/WanSocket.hh"
#include <string>
#include <net/ethernet.h>

#define BUFFER_SIZE 2048

class LanSocket: public ASocket {

public:
  LanSocket(int sock_fd, std::queue<std::string>& to_que, std::queue<std::string>& from_que);
  void handleInEvent(void) override;
  void handleOutEvent(void) override;

};

LanSocket* createLanSocket();