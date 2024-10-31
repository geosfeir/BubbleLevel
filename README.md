Using the Raspberry Pi Pico VSCode extension, this application can be compiled and ran with 2 simple clicks.

This program is a "bubble" level program that turns on NeoMatrix LEDs based off of tilt values read from the
LIS3DH accelerometer on the Adafruit Feather RP2040. 

To build and run the program without the VSCode extension, create a build directory in the parent directory
of the program and run 

cmake ..
cmake --build .
and then run your executable
./executable-name