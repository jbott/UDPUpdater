/* vim: set tabstop=2 shiftwidth=2 expandtab: */
#include "subsystem_communication.h"

Communication::Communication(void)
{
  byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
  local_ip = IPAddress(192,168,1,82);
  remote_ip = IPAddress(192,168,1,81);
  remote_port = 12001;

  Ethernet.begin(mac, local_ip);
  // This is required to initialize the UDP library,
  // even though we do not use the UDP server at all at this point
  ethUDP.begin(remote_port);
}

void
Communication::sendPacket(char *message)
{
  Serial.print("Sending: ");
  Serial.println(message);
  ethUDP.beginPacket(remote_ip, remote_port);
  ethUDP.write(message);
  ethUDP.endPacket();
}
