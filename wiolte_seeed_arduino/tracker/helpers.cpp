#include "helpers.h"

void clear_buffer(char * a_buffer, int a_length){
    for (int i = 0; i < a_length; i++) {
        a_buffer[i] = 0;
    }
}

