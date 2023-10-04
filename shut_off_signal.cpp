#include <iostream>   // Used for cout statements
#include <wiringPi.h> // Include WiringPi library!

#define camera 0 //wiringPi pin 0 = BCM GPIO pin 17 = physical pin 11
#define laser 2 // wiringPi pin 2 = BCM GPIO pin 27 = physical pin 13 


int main(void)
{
    // Setup stuff:
    wiringPiSetup();
    pinMode(camera, OUTPUT);     // Set camera signal
    pinMode(laser, OUTPUT);  
    digitalWrite(laser, 0);
    digitalWrite(camera, 0);    // Falling edge
    std::cout<<"Shut off Signal successfully. \n";     
    return 0;
}
