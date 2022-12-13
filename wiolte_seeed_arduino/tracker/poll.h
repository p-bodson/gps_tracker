#ifndef __PB_POLL_H__
#define __PB_POLL_H__

#include <Arduino.h> 

class Poll {
public:
  Poll(int interval);
  void poll(void(*)());

private:
  uint32_t _interval;  // in milliseconds
  uint32_t _counter;   // in milliseconds
};




#endif
