# Purpose

This software is for the WioLTE board from Seeed Studio. The purpose of this
project is to create a GPS tracker that produces a data stream to be consumed
by another piece of software to visual the path.

# Uploading to WioLTE Board

The WioLTE use DFU to put code onto the board. Here is a reminder to access the
modes.

Normal mode is accessed by pressing the RST button.

DFU mode is accessed by (1) holding BOOT, (2) press and release RST, 
(3) release BOOT.

# Using the serial port

If you are using something like the Arduino IDE to program the board. It is 
best to use another program to view the serial port of the WioLTE. If you
use the IDE to view the serial port, it interferes with the programming
interface.

Picocom is a small, simple program that will do the job.

# License

This project is licensed under 0BSD. See the accompying LICENSE for the exact
copy. This license only applies to the code written for this project.
The license of any libraries used might be different.
