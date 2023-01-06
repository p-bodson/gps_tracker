#include "cellular.h"

Cellular::Cellular() {
    clear_buffer(_apn, sizeof(_apn));
    clear_buffer(_tcp_url, sizeof(_tcp_url));
    clear_buffer(_device_key, sizeof(_device_key));
    clear_buffer(_tags, sizeof(_tags));
    clear_buffer(_data, sizeof(_data));
    _remote_port = 0;
}

void Cellular::set_apn(char * apn) {
    strncpy(_apn, apn, sizeof(_apn));
}

void Cellular::set_tcp_url(char * tcp_url) {
    strncpy(_tcp_url, tcp_url, sizeof(_tcp_url));
}

void Cellular::set_device_key(char * device_key) {
    strncpy(_device_key, device_key, sizeof(_device_key));
}

void Cellular::set_tags(char * tags) {
    strncpy(_tags, tags, sizeof(_tags));
}

void Cellular::set_data(char * data) {
    strncpy(_data, data, sizeof(_data));
}

void Cellular::set_remote_port(int remote_port) {

    if (remote_port > 65535 || remote_port < 0) {
        _remote_port = 0;
    } 
    else {
        _remote_port = remote_port;
    }
}

void Cellular::clear_tags() {
    clear_buffer(_tags, sizeof(_tags));
}

void Cellular::clear_data() {
    clear_buffer(_data, sizeof(_data));
}

void Cellular::_configure_context() {
    char qicsgp_string[sizeof(_apn) + 100];
    sprintf(qicsgp_string,"AT+QICSGP=1,1,\"%s\",\"\",\"\",3", _apn);

    command_modem(qicsgp_string, 0, 0);
}

void Cellular::_activate_context() {
    command_modem("AT+QIACT=1", 0, 0);
}

bool Cellular::configure_modem() {
    
    // Note: if you retry a command too many times and too quickly,
    // the modem will shut down

    // set the TCP context
    _configure_context();

    delay(5000);

    // then activate it
    _activate_context();
    delay(5000);

    return true;
}



bool Cellular::send(char * data, char * tags) {

    set_data(data);
    set_tags(tags);

    char message[1025];
    clear_buffer(message, 1025);
    sprintf(message, "{\"k\":\"%s\",\"d\":\"%s\",\"t\":\"%s\"}", _device_key, _data, _tags);
    
    char preamble[100];
    clear_buffer(preamble, 100);
    sprintf(preamble, "AT+QISEND=0,%d", strlen(message));

    char socket_info[256];
    clear_buffer(socket_info, 256);
    sprintf(socket_info, "AT+QIOPEN=1,0,\"TCP\",\"%s\",%d,0,1", _tcp_url, _remote_port);

    char socket_close[] = "AT+QICLOSE=0,10";


    command_modem(socket_info, 0, 0);
    delay(2000);

    command_modem(preamble, 0, 0);
    delay(2000);
    command_modem(message, 0, 0);

    // wait for message to send
    delay(10000);

    command_modem(socket_close, 0, 0);
    delay(2000);

    clear_data();
    clear_tags();
}

