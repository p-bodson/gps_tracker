//////////////////////////////////////////
// Preamble
//////////////////////////////////////////
#include "wio_tracker.h"
#include <stdio.h>

///////////
// Globals
///////////

WioTracker wio = WioTracker();

//////////////////////////////////////////
// Setup
//////////////////////////////////////////

void setup() {
    wio.Power_On(); }

//////////////////////////////////////////
// Loop
//////////////////////////////////////////

void loop() {
  listen_for_at_commands(); 
}

//////////////////////////////////////////
// Everything Else
//////////////////////////////////////////
void listen_for_at_commands() {
  capture_command(); 
}

void capture_command() {
  check_module_messages();
  check_debug_messages(); 
}

void check_module_messages() {
    // Check any messages from the module serial line
    while (MODULE_PORT.available() > 0) {
        serialDebug.write(MODULE_PORT.read()); 
    }
}

void check_debug_messages() { 
    String static from_usb = "";

    // Feed any input from the keyboard to the module.
    // Transimssion ends with newline
    while (serialDebug.available() > 0) {   
        int c = -1;
        c = serialDebug.read();      
        if (!is_newline(c)) {
            // grab usb char and show to user
            from_usb += (char)c; 
            serialDebug.write('\r');
            serialDebug.write("[at-cmd]$ ");
            serialDebug.write(from_usb.c_str()); 
        }        
        else {
            if (!from_usb.equals("")) {
                serialDebug.println("\r\nResponse ----------");
                sendData(from_usb, 0);
                from_usb = ""; 
            }
        }
    }
}


String sendData(String command, const int timeout) {
    String response = "";
    MODULE_PORT.println(command);

    long int time = millis();
    while ((time + timeout) > millis()) {
        while (MODULE_PORT.available()) {
            char c = MODULE_PORT.read();
            response += c; 
        }
    }
    return response; 
}

bool is_newline(char c) {
  if (c == '\n' || c == '\r') {
    return true; 
  }
  else {
    return false; 
  }
}

bool is_delete(char c) {
  if (c == 0x7f) {
    return true; 
  }
  else {
    return false; 
  }
}

void print_key_to_hex(char c) {
  int len = 10;
  char hex[len];
  for (int i=0; i < len; i++) {
    hex[i] = 0;
  }
  snprintf(hex, len, "%X", c);
  serialDebug.write(hex);
}
