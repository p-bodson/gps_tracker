#include "tracker.h"
#include "env.h"

const int NMEA_SENTENCE_LENGTH = 512;       // make it long enough to store response
char nmea_sentence[NMEA_SENTENCE_LENGTH];   // stores the nmea response from the module


Poll gnss_poll = Poll( 10000 );   // used for non blocking action timing
GnssData gnss_data = GnssData();
GNSS gnss = GNSS();                                     // the interface with the Wio GNSS module
Cellular transport = Cellular();

void check_module_messages() {
    // Check any messages from the module serial line
    while (Serial1.available() > 0) {
        serialDebug.write(Serial1.read()); 
    }
}

void setup() {
    start_gnss(&gnss);
    init_cellular(&transport);
    delay(2000);
}

void loop() {
    check_module_messages();
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
    //clear_buffer(nmea_sentence, NMEA_SENTENCE_LENGTH);
    //if (gnss.NMEA_read_and_save("RMC", nmea_sentence)) {
    //    if(gnss_data.parse_nmea(nmea_sentence)) {
    //        // send data over network
    //        SerialUSB.println("yes");
    //        transport.send(gnss_data.get_data(), "VEHICLE_TRACKING");
    //    }
    //    else {
            SerialUSB.print("...");
            transport.send("testing", "TESTING_DATA");
    //    }
    //}
}

void init_cellular(Cellular * cell) {

    // set message destination and authentication    
    cell->set_apn(ENV_APN);
    cell->set_tcp_url(ENV_TCP_URL);
    cell->set_device_key(ENV_DEVICE_KEY);
    cell->set_remote_port(ENV_REMOTE_PORT);

    // now configure the modem
    if (!cell->configure_modem()) {
        SerialUSB.println("\n\rFailed to start Cellular Transport");
        return;
    };
    SerialUSB.println("\r\nCellular Transport Ready");
}
