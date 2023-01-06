#ifndef __PB_CELLULAR_H__
#define __PB_CELLULAR_H__

#include <Arduino.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "wio_tracker.h"
#include "helpers.h"
#include "modem_interface.h"

class Cellular {
public:
    Cellular();
    bool send(char *, char *);
    bool configure_modem();
    
    void set_apn(char *);
    void set_tcp_url(char *);
    void set_tags(char *);
    void set_data(char *);
    void set_device_key(char *);
    void set_remote_port(int);
    
    void clear_tags();
    void clear_data();

private:
    char _data[512 + 1];
    char _apn[64 + 1];
    char _tcp_url[64 + 1];
    char _device_key[8 + 1];
    char _tags[128 + 1];
    int _remote_port;

    void _activate_context();
    void _configure_context();
};

#endif
