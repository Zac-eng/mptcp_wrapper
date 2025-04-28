#include "WanSocket.hh"

WanSocket::WanSocket(
  int sock_fd,
  std::queue<std::string>& to_que,
  std::queue<std::string>& from_que
): _sock_fd(sock_fd), _to_que(to_que), _from_que(from_que) {}

WanSocket::~WanSocket() {
  close(_sock_fd);
}

WanStream* WanSocket::listenOn() {
  sockaddr_in addr, client_addr;
  memset(&addr, 0, sizeof(addr));
  memset(&client_addr, 0, sizeof(client_addr));
  socklen_t sock_len = sizeof(client_addr);
  addr.sin_family = AF_INET;
  addr.sin_port = htons(PORT);

  if (inet_pton(AF_INET, BIND_IP, &addr.sin_addr) <= 0) {
    return NULL;
  }
  if (bind(_sock_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
    return NULL;
  }
  if (listen(_sock_fd, 10) != 0) {
    return NULL;
  }
  int stream_fd = accept(_sock_fd, reinterpret_cast<struct sockaddr*>(&client_addr), &sock_len);
  if (stream_fd < 0) {
    return NULL;
  }
  return new WanStream(stream_fd, _to_que, _from_que);
}

WanStream* WanSocket::connectTo() {
  sockaddr_in serv_addr;
  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT);
  if (inet_pton(AF_INET, BIND_IP, &serv_addr.sin_addr) <= 0) {
    return NULL;
  }
  if (connect(_sock_fd, reinterpret_cast<sockaddr *>(&serv_addr), sizeof(serv_addr)) < 0) {
    return NULL;
  }
  return new WanStream(_sock_fd, _to_que, _from_que);
}

WanSocket* createWanSocket(std::queue<std::string> to_que, std::queue<std::string> from_que) {
  int sock_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_MPTCP);
  int opt = 1;
  if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) != 0 || set_non_blocking(sock_fd) != 0) {
    close(sock_fd);
    return NULL;
  }
  return new WanSocket(sock_fd, to_que, from_que);
}
