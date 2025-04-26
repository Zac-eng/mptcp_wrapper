#include "WanSocket.hh"

WanSocket::WanSocket(
  int sock_fd,
  std::queue<std::string>& to_que,
  std::queue<std::string>& from_que
): _sock_fd(sock_fd), _to_que(to_que), _from_que(from_que) {}

WanSocket::~WanSocket() {
  close(_sock_fd);
}

WanStream* listen() {

}

WanStream* connect() {

}

WanSocket* createWanSocket() {
  int sock_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_MPTCP);
  
  return NULL;
}
