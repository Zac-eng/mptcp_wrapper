#pragma once

#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/file.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <string>
#include <queue>

// #ifndef IPPROTO_MPTCP
// # define IPPROTO_MPTCP 262
// #endif

class ASocket {

protected:
  int _sock_fd;
  std::queue<std::string>& _to_que;
  std::queue<std::string>& _from_que;

public:
  ASocket(int sock_fd, std::queue<std::string>& to_que, std::queue<std::string>& from_que);
  ~ASocket();
  int getSockFd(void) const;
  int addEpoll(int epoll_fd);
  virtual void handleInEvent(void) = 0;
  virtual void handleOutEvent(void) = 0;

};

int set_non_blocking(int fd);
