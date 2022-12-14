#ifndef __PB_GNSS_DATA_H__
#define __PB_GNSS_DATA_H__

#include <Arduino.h>
#include <string.h>
#include <stdlib.h>

#include "helpers.h"

const int DATA_CAPACITY = 100;

class GnssData {
public:
    GnssData();
    bool parse_nmea(char*);
    char * get_data();
    int get_data_length();

private:
    int _pack(char*, bool);
    void _reset_data();

    char _data[DATA_CAPACITY];
    int _data_length;
};


#endif
