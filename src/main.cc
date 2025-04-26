#include "WanSocket.hh"
#include "LanSocket.hh"
#include <cstring>
#include <string>
#include <queue>
#include <iostream>
#include <exception>

#define MAX_EVENTS 10
#define TIMEOUT 5

int main(int argc, char* argv[]) {
  try {
    std::queue<std::string> lan_que;
    std::queue<std::string> wan_que;
    LanSocket* lan_sock = createLanSocket();
    WanSocket* wan_sock = createWanSocket();
    WanStream* wan_stream;
    int epoll_fd = epoll_create(MAX_EVENTS);
    epoll_event events[MAX_EVENTS];
    int event_count;

    if (argc < 2 || epoll_fd < 0 || wan_sock == NULL || lan_sock == NULL) {
      std::exit(1);
    }
    if (std::strcmp(argv[1], "listen") == 0) {
      wan_stream = wan_sock->listenOn();
    }
    else if (std::strcmp(argv[1], "connect") == 0) {
      wan_stream = wan_sock->connectTo();
    }
    else {
      std::exit(1);
    }
    if (lan_sock->addEpoll(epoll_fd) != 0 || wan_stream->addEpoll(epoll_fd) != 0) {
      std::exit(1);
    }
    while (true) {
      event_count = epoll_wait(epoll_fd, events, MAX_EVENTS, TIMEOUT);
      for (int i = 0; i < event_count; ++i) {
        ASocket* sock = static_cast<ASocket*>(events[i].data.ptr);
        if (events[i].events & EPOLLIN)
          sock->handleInEvent();
        if (events[i].events & EPOLLOUT)
          sock->handleOutEvent();
      }
    }
    return 0;
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}