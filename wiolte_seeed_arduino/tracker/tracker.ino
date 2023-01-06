#include "tracker.h"
#include "env.h"

const int NMEA_SENTENCE_LENGTH = 512;       // make it long enough to store response
char nmea_sentence[NMEA_SENTENCE_LENGTH];   // stores the nmea response from the module


Poll gnss_poll = Poll( 10000 );   // used for non blocking action timing
GnssData gnss_data = GnssData();
GNSS gnss = GNSS();                                     // the interface with the Wio GNSS module
Cellular transport = Cellular();

void setup() {
    start_gnss(&gnss);
    init_cellular(&transport);
}

void loop() {
    gnss_poll.poll(&get_gnss_data);
}

void start_gnss(GNSS * module) {
    module->Power_On();

    while (!gnss.Check_If_Power_On()) {
        serialDebug.println("Waiting for module to power on...");
        delay(1000);
    }
    serialDebug.println("\n\rPower On!");

    command_modem("AT+QGPS=1", 0, 0);
    delay(5000);
    command_modem("AT+QGPSCFG=\"nmeasrc\",1", 0, 0);
    delay(5000);

}

void get_gnss_data() {
    clear_buffer(nmea_sentence, NMEA_SENTENCE_LENGTH);
    command_modem("AT+QGPSGNMEA=\"RMC\"", nmea_sentence, NMEA_SENTENCE_LENGTH);
    if(gnss_data.parse_nmea(nmea_sentence)) {
        serialDebug.print("Sending: ");
        serialDebug.println(gnss_data.get_data());
        //transport.send(gnss_data.get_data(), "VEHICLE_TRACKING");
    }
    else {
        serialDebug.print("Bad parse: ");
        serialDebug.println(nmea_sentence);
    }
}

void init_cellular(Cellular * cell) {

    // set message destination and authentication    
    cell->set_apn(ENV_APN);
    cell->set_tcp_url(ENV_TCP_URL);
    cell->set_device_key(ENV_DEVICE_KEY);
    cell->set_remote_port(ENV_REMOTE_PORT);

    // now configure the modem
    cell->configure_modem();
}
