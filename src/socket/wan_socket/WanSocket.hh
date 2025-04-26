#pragma once

#include "ASocket.hh"
#include "WanStream.hh"
#include <sys/socket.h>

class WanSocket {

private:
  int _sock_fd;
  std::queue<std::string>& _to_que;
  std::queue<std::string>& _from_que;

public:
  WanSocket(int sock_fd, std::queue<std::string>& to_que, std::queue<std::string>& from_que);
  ~WanSocket();
  WanStream* listen();
  WanStream* connect();

};

WanSocket* createWanSocket();
