#pragma once

#include "ASocket.hh"
#include "epoll_handler/EpollHandler.hh"

class WanListenSocket: public ASocket {

public:
  WanListenSocket(int sock_fd);
  void handleInEvent() override;
  void handleOutEvent() override;
};

WanListenSocket* createWanListenSocket();
