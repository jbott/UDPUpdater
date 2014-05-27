/* vim: set tabstop=2 shiftwidth=2 expandtab: */
#include "subsystem_touch.h"

Touch::Touch(void)
{
  cs = new CapacitiveSensor(CS_SEND_PIN, CS_RECV_PIN);
  lastVal = 0;
  risingEdgeTime = millis();
  longSent = false;
}

void
Touch::run()
{
  long sensedVal = cs->capacitiveSensor(3);

  if (sensedVal > TOUCH_MIN_VAL) {
    // Sensed touch
    if (lastVal <= TOUCH_MIN_VAL) {
      // Last was low
      risingEdgeTime = millis();
    }

    if ((millis() - risingEdgeTime) > TOUCH_MILLIS_LONG) {
      // Long press
      if (!longSent) {
        // Only send long press once
        longSent = true;
        comm->sendPacket(PACKET_TOUCH_LONG);
      }
    }
  } else {
    // Did not sense touch
    if (lastVal > TOUCH_MIN_VAL) {
      // Last was high
      unsigned long touchTime = millis() - risingEdgeTime;
      if (touchTime <= TOUCH_MILLIS_LONG && touchTime > TOUCH_MILLIS_SHORT) {
        // Short press
        comm->sendPacket(PACKET_TOUCH);
      }
    }

    // Clear long sent flag
    longSent = false;
  }

  lastVal = sensedVal;
}
