#include "gnss_data.h"

GnssData::GnssData() {
    _data_length = 0;
    clear_buffer(_data, DATA_CAPACITY);
}

bool GnssData::parse_nmea(char * sentence) {
    char * save_ptr;
    char * delimiters = ",";
    
    char * nmea_type = strtok_r(sentence, delimiters, &save_ptr); 
    if (strncmp(nmea_type, "$GPRMC", 6) != 0) {
        // not using RMC sentence
        return false;
    }
    
    char * utc_time = strtok_r(NULL, delimiters, &save_ptr);  // utc time

    char * status = strtok_r(NULL, delimiters, &save_ptr); 
    if (strncmp(status, "A", 1) != 0) {
        // invalid data
        return false;
    }
    else {
        _reset_data();

        _pack(utc_time, false);
        _pack(strtok_r(NULL, delimiters, &save_ptr), false);    // latitude 
        _pack(strtok_r(NULL, delimiters, &save_ptr), false);    // north or south
        _pack(strtok_r(NULL, delimiters, &save_ptr), false);    // longitude
        _pack(strtok_r(NULL, delimiters, &save_ptr), false);    // east or west
        _pack(strtok_r(NULL, delimiters, &save_ptr), false);    // speed over ground
        _pack(strtok_r(NULL, delimiters, &save_ptr), false);    // course over ground
        _pack(strtok_r(NULL, delimiters, &save_ptr), false);    // utc date
        _pack(strtok_r(NULL, delimiters, &save_ptr), false);    // magnetic variation
        _pack(strtok_r(NULL, delimiters, &save_ptr), true);     // east or west
  
        return true;
    }

    return false;
}

int GnssData::_pack(char * some_data, bool last_data) {
    char c;
    int char_written = 0;

    // don't overwrite last null byte
    if (_data_length == DATA_CAPACITY-1) {
        return 0;
    }

    // otherwise write data
    while (_data_length < DATA_CAPACITY-1) {
        c = some_data[char_written];
        if (c != 0) {
            _data[_data_length] = c;
            _data_length += 1;
            char_written += 1;
        }
        else {
            break;
        }
    }
  
    // no need for delimter at last piece of data
    if (!last_data) {
        _data[_data_length] = ',';
        _data_length += 1;
        char_written += 1;
    }

    return char_written;
}

void GnssData::_reset_data() {
    clear_buffer(_data, DATA_CAPACITY);
    _data_length = 0;
}

char * GnssData::get_data() {
    return _data;
}

int GnssData::get_data_length() {
    return _data_length;
}
