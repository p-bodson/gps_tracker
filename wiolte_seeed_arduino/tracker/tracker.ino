#include "gnss.h"
#include "poll.h"
#include <string.h>

struct GnssData {  
  char utc_time[11];         // hhmmss.sss
  char status_1;             // A = active, V = void
  char latitude[10];         // ddmm.mmmm
  char ns_indicator;         // north or south
  char longitude[11];        // dddmm.mmmm
  char ew_indicator;         // east or west
  float speed_over_ground;   // knots
  float course_over_ground;  // degrees
  char utc_date[7];          // DDMMYY
  float magnetic_variation;  // degrees
  char mv_indicator;         // magnetic variation indicator; east or west
  char status_2;             // A = alright, N = no good
  char checksum[4];          // *25
};

// TODO move sentence into class
// TODO move gnss data into class


const int nmea_sentence_length = 512;     // make it long enough to store response
char nmea_sentence[nmea_sentence_length]; // stores the nmea response from the module
Poll gnss_poll(2000); // used for non blocking action timing

// the interface with the Wio GNSS module
GNSS gnss = GNSS();
  
void setup() {
    start_gnss(&gnss);
    delay(2000);
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

void loop() {
    gnss_poll.poll(&get_gnss_data);
}

void get_gnss_data() {
    clear_buffer(nmea_sentence, nmea_sentence_length);
    if (gnss.NMEA_read_and_save("RMC", nmea_sentence)) {
      parse_nmea_sentence(nmea_sentence, nmea_sentence_length);
    }
}

void parse_nmea_sentence(char *sentence, int s_length) {
      SerialUSB.println(sentence);
}

void clear_buffer(char * a_buffer, int a_length){
  for (int i = 0; i < a_length; i++) {
    a_buffer[i] = 0;
  }
}
