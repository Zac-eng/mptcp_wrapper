#include "LanSocket.hh"

LanSocket::LanSocket(int sock_fd): ASocket(sock_fd) {}

LanSocket* createLanSocket(const char* intf_name) {
  EpollHandler& handler = EpollHandler::getInstance();

  int sock_fd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
  if (sock_fd < 0) {
    return NULL;
  }
  if (setsockopt(sock_fd, SOL_SOCKET, SO_BINDTODEVICE, intf_name, strlen(intf_name)) < 0) {
    close(sock_fd);
    return NULL;
  }
  LanSocket* socket = new LanSocket(sock_fd);
  handler.registerSocket(socket, EPOLLIN);
  return socket;
}

void LanSocket::handleInEvent() {
  std::string message;
  char buffer[BUFFER_SIZE];

  while (true) {
    ssize_t n = recvfrom(_sock_fd, buffer, BUFFER_SIZE - 1, 0, NULL, NULL);
    if (n < 0) {
      perror("recvfrom");
      std::exit(1);
    }
    buffer[n] = '\0';
    message += buffer;
    if (n < BUFFER_SIZE)
      break;
  }
  createWanConnectSocket(message);
}
