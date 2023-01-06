#include "modem_interface.h"

void command_modem(char * command, char * save_buffer, int save_length) {   
    const int interchar_timeout = 1000;
    
    // send commmand to modem
    MODULE_PORT.println(command);

    // wait indefinitely for modem to responsd    
    while (MODULE_PORT.available() == 0);

    // read response to debug when character received until
    // timeout is reached
    int save_index = 0;
    unsigned long last_read = millis();
    while (millis() - last_read < interchar_timeout ) {
        while(MODULE_PORT.available()) {
            char c = MODULE_PORT.read();          
            serialDebug.write(c);
            last_read = millis();

            // save response;
            if (save_index < save_length) {
                save_buffer[save_index] = c;
                save_index += 1;
            }
        }
    }
}