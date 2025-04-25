#include "WanConnectSocket.hh"

WanConnectSocket::WanConnectSocket(int sock_fd, const std::string& message): ASocket(sock_fd), _message(message) {}

WanConnectSocket* createWanConnectSocket(const std::string& message) {
  EpollHandler& handler = EpollHandler::getInstance();

  int sock_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_MPTCP);
  if (sock_fd < 0) {
    return NULL;
  }
  WanConnectSocket* wan_sock = new WanConnectSocket(sock_fd, message);
  handler.registerSocket(wan_sock, EPOLLOUT);
  return wan_sock;
}

void WanConnectSocket::handleOutEvent() {
  EpollHandler& handler = EpollHandler::getInstance();
  sockaddr_in server_addr;
  memset(&server_addr, 0, sizeof(server_addr));
  const char* server_ip = "127.0.0.1";
  const int server_port = 8080;

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(server_port);
  if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) > 0) {
    if (connect(_sock_fd, reinterpret_cast<struct sockaddr*>(&server_addr), sizeof(server_addr)) == 0) {
      send(_sock_fd, _message.c_str(), _message.length(), 0);
    }
  }
  handler.unregisterSocket(this);
  delete this;
}
