/* vim: set tabstop=2 shiftwidth=2 expandtab: */
#ifndef MOTION_H
#define MOTION_H
#include "hardware.h"
#include "subsystem_communication.h"

#define MOTION_MILLIS_MIN         200

class Motion {
  public:
    Motion(void);
    void run(void);

  private:
    bool lastVal;
    unsigned long risingEdgeTime;
};

#endif
