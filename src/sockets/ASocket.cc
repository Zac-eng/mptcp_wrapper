#include "ASocket.hh"

ASocket::ASocket(int sock_fd): _sock_fd(sock_fd) {}

ASocket::~ASocket() {
  close(_sock_fd);
}

int ASocket::getSockFd() const {
  return _sock_fd;
}
