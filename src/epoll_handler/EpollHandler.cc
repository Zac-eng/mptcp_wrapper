#include "EpollHandler.hh"

EpollHandler::EpollHandler() {
  _epoll_fd = epoll_create(1);
  if (_epoll_fd == -1) {
    throw std::runtime_error("failed to create epoll fd");
  }
}

EpollHandler::~EpollHandler() {
  std::map<int, ASocket*>::iterator it;
  close(_epoll_fd);
  while (it != _epoll_map.end()) {
    delete it->second;
    ++it;
  }
}

EpollHandler& EpollHandler::getInstance() {
  static EpollHandler instance;
  return instance;
}

int EpollHandler::registerSocket(ASocket* socket, uint32_t events) {
  epoll_event event;
  int fd = socket->getSockFd();
  event.events = events;
  event.data.fd = fd;
  if (epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, fd, &event) == -1) {
    return -1;
  }
  if (_epoll_map.insert(std::make_pair(fd, socket)).second == false) {
    epoll_ctl(_epoll_fd, EPOLL_CTL_DEL, fd, NULL);
    return -1;
  }
  return 0;
}

int EpollHandler::modifySocket(ASocket* socket, uint32_t events) {
  epoll_event event;
  int fd = socket->getSockFd();
  event.events = events;
  event.data.fd = fd;
  if (epoll_ctl(_epoll_fd, EPOLL_CTL_MOD, fd, &event) == -1) {
    return -1;
  }
  return 0;
}

int EpollHandler::unregisterSocket(ASocket* socket) {
  int status = 0;

  int fd = socket->getSockFd();
  std::map<int, ASocket*>::iterator to_delete = _epoll_map.find(fd);
  if (to_delete != _epoll_map.end()) {
    delete to_delete->second;
    _epoll_map.erase(to_delete);
  }
  return epoll_ctl(_epoll_fd, EPOLL_CTL_DEL, fd, NULL);
}

int EpollHandler::poll() {
  epoll_event events[MAX_EVENTS];

  int event_count = epoll_wait(_epoll_fd, events, MAX_EVENTS, TIMEOUT);
  if (event_count == -1) {
      throw std::runtime_error("epoll_wait failed");
  }
  for (int i = 0; i < event_count; ++i) {
    std::map<int, ASocket*>::iterator sock = _epoll_map.find(events->data.fd);
    if (sock == _epoll_map.end())
      continue;
    if (events[i].events == EPOLLIN) {
      sock->second->handleInEvent();
    }
    else if (events[i].events == EPOLLOUT) {
      sock->second->handleOutEvent();
    }
  }
  return event_count;
}

int set_non_blocking(int fd) {
  int flag;

	flag = fcntl(fd, F_GETFL);
	if (flag == -1)
		return -1;
	flag |= O_NONBLOCK;
	if (fcntl(fd, F_SETFL, flag) == -1)
		return -1;
	return 0;
}
