/* vim: set tabstop=2 shiftwidth=2 expandtab: */
#ifndef TOUCH_H
#define TOUCH_H
#include <CapacitiveSensor.h>
#include "hardware.h"
#include "subsystem_communication.h"

#define TOUCH_MIN_VAL            5

#define TOUCH_MILLIS_SHORT       75
#define TOUCH_MILLIS_LONG        500

class Touch {
  public:
    Touch(void);
    void run(void);

  private:
    CapacitiveSensor *cs;
    int lastVal;
    unsigned long risingEdgeTime;
    bool longSent;
};

#endif
