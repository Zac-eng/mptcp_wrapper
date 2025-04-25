#include "WanListenSocket.hh"

WanListenSocket::WanListenSocket(int sock_fd): ASocket(sock_fd) {}

WanListenSocket* createWanListenSocket() {
  EpollHandler& handler = EpollHandler::getInstance();

  sockaddr_in server_addr;
  memset(&server_addr, 0, sizeof(server_addr));
  const char* server_ip = "127.0.0.1";
  const int server_port = 8080;
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(server_port);
  if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) <= 0) {
    return NULL;
  }

  int sock_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_MPTCP);
  if (sock_fd < 0) {
    return NULL;
  }
  int opt = 1;
  if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<char *>(&opt), sizeof(opt)) >= 0) {
    if (bind(sock_fd, reinterpret_cast<struct sockaddr *>(&server_addr), sizeof(server_addr)) >= 0) {
      if (listen(sock_fd, 10) >= 0) {
        WanListenSocket* wan_sock = new WanListenSocket(sock_fd);
        handler.registerSocket(wan_sock, EPOLLIN);
        return wan_sock;
      }
    }
  }
  close(sock_fd);
  return NULL;
}

void WanListenSocket::handleInEvent() {
  sockaddr_in server_addr;

  int client_sock = accept(_sock_fd, reinterpret_cast<struct sockaddr *>(&server_addr), sizeof(server_addr));
  if (client_sock < 0) {
    return;
  }
}
