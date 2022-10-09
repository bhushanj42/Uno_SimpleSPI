#ifndef SUPPORT_H // include guard
#define SUPPORT_H

#define INPUT     0
#define OUTPUT    1

void SPIStateMachine();

enum {
  PrepareDataToSend,
  WaitForReception
};

#endif
