#pragma once

#include "ASocket.hh"
#include <stdexcept>

#define BUFFER_SIZE 2048

class WanStream: public ASocket {

public:
  WanStream(int sock_fd, std::queue<std::string>& to_que, std::queue<std::string>& from_que);
  void handleInEvent(void);
  void handleOutEvent(void);
};
