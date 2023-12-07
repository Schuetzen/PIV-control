#include <stdio.h>    // Used for printf() statements
#include <iostream>   // Used for cout statements
#include <wiringPi.h> // Include WiringPi library!

#define camera 0 //wiringPi pin 0 = BCM GPIO pin 17 = physical pin 11
#define laser 2 // wiringPi pin 2 = BCM GPIO pin 27 = physical pin 13 

const int dt = 10; // verify the maximal time interval allowed. 50Hz->20ms
const int flash = 1;
const int freq = 10;
const int exposure = 7; // 10 ms of camera exposure time
const int laser_dur = 2;

int main(void)
{
    // Setup stuff:
    wiringPiSetup();
    pinMode(camera, OUTPUT);     // Set camera signal
    pinMode(laser, OUTPUT);

while(true){
    digitalWrite(laser, 1);
    delay(laser_dur);
    digitalWrite(laser, 0);
    delay(1);
    
    digitalWrite(laser, 1);
    delay(laser_dur);
    digitalWrite(laser, 0);
    delay(500);
}


    std::cout << "Signal is On!\n";

    return 0;
}
