#include "poll.h"

Poll::Poll(int interval) {
    _interval = interval;
    _counter = 0;
}

void Poll::poll (void (*polled_action) ()) {
  if ( millis() - _counter < _interval) {
   return;
  }
  else {
    polled_action();
    _counter = millis();
  }
}
