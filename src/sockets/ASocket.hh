#pragma once

#include <sys/socket.h>
#include <netinet/in.h>
#include <cstdlib>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

// #ifndef IPPROTO_MPTCP
// # define IPPROTO_MPTCP 262
// #endif

class ASocket {

protected:
  int _sock_fd;

public:
  ASocket(int sock_fd);
  ~ASocket();
  int getSockFd(void) const;
  virtual void handleInEvent() = 0;
  virtual void handleOutEvent() = 0;

};
