#pragma once

#include "ASocket.hh"
#include <sys/epoll.h>
#include <sys/file.h>
#include <map>
#include <unistd.h>
#include <iterator>
#include <stdexcept>

#define MAX_EVENTS 10
#define TIMEOUT 5

class EpollHandler {

private:
  int _epoll_fd;
  std::map<int, ASocket*> _epoll_map;
  EpollHandler();
  ~EpollHandler();

public:
  EpollHandler(const EpollHandler&) = delete;
  EpollHandler& operator=(const EpollHandler&) = delete;
  static EpollHandler& getInstance();
  int poll(void);
  int registerSocket(ASocket* socket, uint32_t events);
  int modifySocket(ASocket* socket, uint32_t events);
  int unregisterSocket(ASocket* socket);

};

int set_non_blocking(int fd);
