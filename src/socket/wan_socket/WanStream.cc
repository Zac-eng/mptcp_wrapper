#include "WanStream.hh"

WanStream::WanStream(
  int sock_fd,
  std::queue<std::string>& to_que,
  std::queue<std::string>& from_que
): ASocket(sock_fd, to_que, from_que) {}

void WanStream::handleInEvent(void) {
  std::string buffer;
  char buf[BUFFER_SIZE];

  while (true) {
    ssize_t bytes_received = read(_sock_fd, buf, BUFFER_SIZE - 1);
    if (bytes_received < 0)
      throw std::runtime_error("failed to receive");
    buf[bytes_received] = '\0';
    buffer += buf;
    if (bytes_received < BUFFER_SIZE - 1)
      break;
  }
  _to_que.push(buffer);
}

void WanStream::handleOutEvent(void) {
  if (_from_que.empty())
    return;
  const char *message = _from_que.front().c_str();
  _from_que.pop();
  ssize_t bytes_sent = write(_sock_fd, message, std::strlen(message));
  if (bytes_sent < 0)
    throw std::runtime_error("failed to send");
}
