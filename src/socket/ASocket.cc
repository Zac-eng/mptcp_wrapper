#include "ASocket.hh"

ASocket::ASocket(
  int sock_fd,
  std::queue<std::string>& to_que,
  std::queue<std::string>& from_que
): _sock_fd(sock_fd), _to_que(to_que), _from_que(from_que) {}

ASocket::~ASocket() {
  close(_sock_fd);
}

int ASocket::addEpoll(int epoll_fd) {
  epoll_event ev;
  ev.events = EPOLLIN || EPOLLOUT;
  ev.data.fd = _sock_fd;
  ev.data.ptr = static_cast<void*>(this);
  return epoll_ctl(epoll_fd, EPOLL_CTL_ADD, _sock_fd, &ev);
}

int ASocket::getSockFd() const {
  return _sock_fd;
}