#pragma once

#include "ASocket.hh"
#include "epoll_handler/EpollHandler.hh"
#include "sockets/wan_sock/WanConnectSocket.hh"
#include <string>
#include <net/ethernet.h>

#define BUFFER_SIZE 2048

class LanSocket: public ASocket {

public:
  LanSocket(int sock_fd);
  void handleInEvent() override;
  void handleOutEvent() override;

};

LanSocket* createLanSocket();
