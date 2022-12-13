#include "gnss.h"
#include "timed_command.h"


typedef struct Poll {
  volatile uint32_t delay ;
  volatile uint32_t counter;
} Poll;

static Poll gps_poll = {2000, 0};
void pollAction (void (*polled_action) (), Poll * poll) {
  if ( millis() - poll->counter < poll->delay) {
   return;
  }
  else {

    polled_action();

    poll->counter = millis();
  }
}


// Instance of GNSS class
GNSS gnss = GNSS();
Timer timer = TimedCommand();
  
void setup() {
    // Module power on 
    gnss.Power_On();
    while (!gnss.Check_If_Power_On()) {
        SerialUSB.println("Waitting for module to power on...");
        delay(1000);
    }
    SerialUSB.println("\n\rPower On!");

    if (!(gnss.open_GNSS())) {
        SerialUSB.println("\n\rGNSS init failed!");
        return;
    }
    SerialUSB.println("Open GNSS OK.");
    delay(2000);
    SerialUSB.println(timer.test());
}

void loop() {
    pollAction(&gnss.dataFlowMode, &gps_poll;
}
