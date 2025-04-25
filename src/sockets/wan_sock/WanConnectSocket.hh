#pragma once

#include "ASocket.hh"
#include "EpollHandler.hh"
#include <string>

class WanConnectSocket: public ASocket {

private:
  std::string _message;

public:
  WanConnectSocket(int sock_fd, const std::string& message);
  ~WanConnectSocket();
  void handleInEvent() override;
  void handleOutEvent() override;

};

WanConnectSocket* createWanConnectSocket(const std::string& message);
