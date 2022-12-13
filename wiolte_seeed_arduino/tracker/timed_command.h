#ifndef __PB_TIMED_COMMAND_H__
#define __PB_TIMED_COMMAND_H__

#include <Arduino.h> 
#include  <stdio.h>

class TimedCommand {
public:
    enum TimerType {ONE=1, TWO, THREE};
    char test();
};

#endif
