#include "LanSocket.hh"

LanSocket::LanSocket(int sock_fd,
  std::queue<std::string>& to_que,
  std::queue<std::string>& from_que
): ASocket(sock_fd, to_que, from_que) {}

LanSocket* createLanSocket(
  char const* intf_name,
  std::queue<std::string>& to_que,
  std::queue<std::string>& from_que
) {
  int sock_fd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
  if (sock_fd < 0) {
    perror("raw sock creation");
    return NULL;
  }
  if (setsockopt(sock_fd, SOL_SOCKET, SO_BINDTODEVICE, intf_name, strlen(intf_name)) < 0 \
    || set_non_blocking(sock_fd) != 0) {
      perror("socket option");
    close(sock_fd);
    return NULL;
  }
  return new LanSocket(sock_fd, to_que, from_que);
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
  _to_que.push(message);
}

void LanSocket::handleOutEvent(void) {
  sockaddr_in dest_addr;
  memset(&dest_addr, 0, sizeof(dest_addr));
  dest_addr.sin_family = AF_INET;
  if (inet_pton(AF_INET, BIND_IP, &dest_addr.sin_addr) != 1) {
    perror("inet_pton");
    return;
  }
  if (_from_que.empty())
    return;
  const char* message = _from_que.front().c_str();
  _from_que.pop();
  ssize_t bytes_sent = sendto(_sock_fd, message, sizeof(message), 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
  if (bytes_sent < 0) {
    std::exit(EXIT_FAILURE);
  }
}
