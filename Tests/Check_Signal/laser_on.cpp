#include <stdio.h>    // Used for printf() statements
#include <iostream>   // Used for cout statements
#include <wiringPi.h> // Include WiringPi library!

#define camera 0 //wiringPi pin 0 = BCM GPIO pin 17 = physical pin 11
#define laser 2 // wiringPi pin 2 = BCM GPIO pin 27 = physical pin 13 

const int dt = 10; // verify the maximal time interval allowed. 50Hz->20ms
const int flash = 1;
const int freq = 10;
const int exposure = 7; // 10 ms of camera exposure time

int main(void)
{
    // Setup stuff:
    wiringPiSetup();
    pinMode(camera, OUTPUT);     // Set camera signal
    pinMode(laser, OUTPUT);

        // turn on laser
        // digitalWrite(laser, 1);
        // turn on camera for the first image
        // digitalWrite(camera, 1); // Rising edge 
        // delay(flash); // g
        // digitalWrite(camera, 0); // Falling edge
        // delay(dt-flash); // wait delt_t for the second signal
        
        // turn on camera for the second image
        // digitalWrite(camera, 1); // Rising edge 
        // delay(flash+exposure); // 
        // digitalWrite(camera, 0);
    digitalWrite(laser, 1);
        // delay(1000/freq-flash-dt-exposure); // wait until the next pair
    
    std::cout << "Signal is On!\n";

    return 0;
}
