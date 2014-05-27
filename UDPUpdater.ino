/* vim: set tabstop=2 shiftwidth=2 expandtab: */
#include <CapacitiveSensor.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <IRremote.h>
#include <PString.h>
#include <SPI.h>
#include "hardware.h"
#include "subsystem_communication.h"
#include "subsystem_motion.h"
#include "subsystem_remote.h"
#include "subsystem_touch.h"

Communication *comm;
Motion *motion;
Remote *remote;
Touch *touch;

void setup() {
  // Start Serial
  Serial.begin(9600);
  Serial.println("UPDUpdater");
  Serial.print("Built: ");
  Serial.println(__TIMESTAMP__);

  // Start communications first
  comm = new Communication();

  // Individual sensor subsystems
  motion = new Motion();
  remote = new Remote();
  touch = new Touch();

  Serial.println("setup() complete");
}

void loop() {
  // Update each sensor subsystem one after each other
  motion->run();
  remote->run();
  touch->run();
}
