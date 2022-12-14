#include "tracker.h"

const int NMEA_SENTENCE_LENGTH = 512;       // make it long enough to store response
char nmea_sentence[NMEA_SENTENCE_LENGTH];   // stores the nmea response from the module

Poll gnss_poll = Poll(2000);                // used for non blocking action timing
GnssData gnss_data = GnssData();
GNSS gnss = GNSS();                         // the interface with the Wio GNSS module
  
void setup() {
    start_gnss(&gnss);
    delay(2000);
}

void loop() {
    gnss_poll.poll(&get_gnss_data);
}

void start_gnss(GNSS * module) {
    module->Power_On();
    while (!gnss.Check_If_Power_On()) {
        SerialUSB.println("Waiting for module to power on...");
        delay(1000);
    }
    SerialUSB.println("\n\rPower On!");

    if (!(module->open_GNSS())) {
        SerialUSB.println("\n\rGNSS init failed");
        return;
    }
    SerialUSB.println("Open GNSS OK.");
    
    if (!(module->enable_NMEA_mode())) {
        SerialUSB.println("\n\rFailed to enable NMEA");
        return;
    }
    SerialUSB.println("Using NMEA");
}

void get_gnss_data() {
    clear_buffer(nmea_sentence, NMEA_SENTENCE_LENGTH);
    if (gnss.NMEA_read_and_save("RMC", nmea_sentence)) {
        if(gnss_data.parse_nmea(nmea_sentence)) {
            // send data over network        
        }
    }
}

