/* vim: set tabstop=2 shiftwidth=2 expandtab: */
#include "subsystem_motion.h"

Motion::Motion(void)
{
  pinMode(PIR_PIN, INPUT);
  lastVal = false;
  risingEdgeTime = millis();
}

void
Motion::run()
{
  bool sensedVal = digitalRead(PIR_PIN);

  if (sensedVal) {
    // Sensed motion
    if (!lastVal) {
      // Last was false
      risingEdgeTime = millis();
    }

    if ((millis() - risingEdgeTime) > MOTION_MILLIS_MIN) {
      // Motion detected
      comm->sendPacket(PACKET_MOTION);

      // Reset timer to act as an interval
      risingEdgeTime = millis();
    }
  }

  lastVal = sensedVal;
}
