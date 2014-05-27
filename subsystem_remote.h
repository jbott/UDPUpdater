/* vim: set tabstop=2 shiftwidth=2 expandtab: */
#ifndef IRREMOTE_H
#define IRREMOTE_H
#include <IRremote.h>
#include "hardware.h"
#include "subsystem_communication.h"

class Remote {
  public:
    Remote(void);
    void run(void);

  private:
    IRrecv *receive;
    decode_results results;
    char replyBuffer[PACKET_SIZE];
    PString *replyString;
};

#endif
