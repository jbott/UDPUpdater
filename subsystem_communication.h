/* vim: set tabstop=2 shiftwidth=2 expandtab: */
#ifndef COMMUNICATION_H
#define COMMUNICATION_H
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <PString.h>
#include <SPI.h>
#include "hardware.h"

#define PACKET_SIZE              128
#define PACKET_IR                "IRCODE="
#define PACKET_MOTION            "MOTION"
#define PACKET_TOUCH             "TOUCH"
#define PACKET_TOUCH_LONG        "LONGTOUCH"

class Communication {
  public:
    Communication(void);
    void sendPacket(char *message);

  private:
    byte mac[];
    IPAddress local_ip;
    IPAddress remote_ip;
    int remote_port;
    EthernetUDP ethUDP;
};

// Allow all classes to access the main communication object
extern Communication *comm;
#endif
