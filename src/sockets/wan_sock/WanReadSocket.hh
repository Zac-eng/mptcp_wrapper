#pragma once

#include "ASocket.hh"

class WanReadSocket: public ASocket {

public:
  void handleInEvent() override;
  void handleOutEvent() override;

};

WanReadSocket* createWanReadSocket();
