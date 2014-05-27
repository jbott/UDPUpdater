/* vim: set tabstop=2 shiftwidth=2 expandtab: */
#include "subsystem_remote.h"

Remote::Remote(void)
{
  receive = new IRrecv(IR_PIN);
  receive->enableIRIn();
  replyString = new PString(replyBuffer, sizeof(replyBuffer));
}

void
Remote::run(void)
{
  if (receive->decode(&results)) {
    if (results.decode_type == NEC) {
      // Filters out bad IR receives
      // Append the recieved code to the end of the packet
      replyString->begin();
      replyString->print(PACKET_IR);
      replyString->print(results.value, HEX);
      comm->sendPacket(replyBuffer);
    }
    // Recieve the next value
    receive->resume();
  }
}
