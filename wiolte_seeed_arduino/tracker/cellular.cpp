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

bool Cellular::configure_modem() {

    //TODO: Handle response from modem other than ERROR and OK

    // set TCP context
    char qicsgp_string[sizeof(_apn) + 100];
    sprintf(qicsgp_string,"AT+QICSGP=1,1,\"%s\",\"\",\"\",3\r\n", _apn);
    
    const int retry_max = 3;
    int retry_count = 0;
    
    while (retry_count < retry_max) {
        send_cmd(qicsgp_string);
        retry_count += 1;
        delay(1000);   
    }

    // activate the context
    retry_count = 0;
    while (retry_count < retry_max) {
        send_cmd("AT+QIACT=1\r\n");
        retry_count += 1;
        delay(1000);
    }

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

    //SerialUSB.print(socket_info);
    send_cmd(socket_info);
    delay(1000);
    //SerialUSB.print(preamble);
    send_cmd(preamble);
    delay(1000);
    //SerialUSB.print(message);
    send_cmd(message);

    // wait for message to send
    delay(5000);

    //SerialUSB.print(socket_close);
    send_cmd(socket_close);
    delay(1000);

    clear_data();
    clear_tags();
}

