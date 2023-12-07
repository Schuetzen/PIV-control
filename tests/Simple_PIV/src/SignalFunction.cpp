#include "SignalFunction.h"
#include "UtilityFunctions.h"
#include <wiringPi.h>
#include <chrono>

using namespace std;


// Implementation of Signal function
void* Signal(void* arg) {
    int* params = (int*) arg;
    int exposure = params[0];
    int dur = params[1];
    int freq = params[2];
    int dt = params[3];
    
    wiringPiSetup();
    int cameraPin = 0;    
    int laserPin = 2;    

    pinMode(cameraPin, OUTPUT);    
    pinMode(laserPin, OUTPUT);

    auto start_time = chrono::steady_clock::now(); // Now std::chrono:: is correctly recognized
    while(chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now() - start_time).count() < dur) {
        PrintCurrentTimeAndMessage("Laser is running!"); // Function is now recognized
 
        digitalWrite(laserPin, 1);
        digitalWrite(cameraPin, 1);
        delay(FLASH_DURATION);
        digitalWrite(cameraPin, 0);
        delay(dt - FLASH_DURATION);
        digitalWrite(cameraPin, 1); 
        delay(FLASH_DURATION + exposure);
        digitalWrite(cameraPin, 0);
        digitalWrite(laserPin, 0);
        delay(1000/freq - FLASH_DURATION - dt - exposure);
    }
    return nullptr;
}

