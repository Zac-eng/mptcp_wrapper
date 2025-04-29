#pragma once

#include "socket/ASocket.hh"
#include "WanStream.hh"
#include <sys/socket.h>

#define PORT 4200
#define BIND_IP "192.168.32.1"

class WanSocket {

private:
  int _sock_fd;
  std::queue<std::string>& _to_que;
  std::queue<std::string>& _from_que;

public:
  WanSocket(int sock_fd, std::queue<std::string>& to_que, std::queue<std::string>& from_que);
  ~WanSocket();
  WanStream* listenOn();
  WanStream* connectTo();

};

WanSocket* createWanSocket(std::queue<std::string> to_que, std::queue<std::string> from_que);
