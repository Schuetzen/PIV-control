// Include necessary headers (to use Pylon API)
#include <pylon/PylonIncludes.h>
#include <pylon/ParameterIncludes.h>
#include <pylon/BaslerUniversalInstantCamera.h>

// Include WiringPi library!
#include <wiringPi.h>

// Include necessary headers
#include <iostream>
#include <ctime>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/stat.h>
#include <sys/types.h>
// Include for config read
#include <fstream>
#include <unordered_map>

#include <thread>


// Use the Pylon namespace and the standard namespace
using namespace Pylon;
using namespace std;

// Signal headfile


//wiringPi pin 0 = BCM GPIO pin 17 = physical pin 11
#define cameraPin 0    
// wiringPi pin 2 = BCM GPIO pin 27 = physical pin 13 
#define laserPin 2     

// verify the maximal time interval allowed. 50Hz->20ms
// const int dt = 10;  

const int flash = 1;
const int freq = 10;
// 10 ms of camera exposure time
//const int exposure = 7; 
// int exposure;

// Read config File
std::unordered_map<std::string, std::string> readConfigFile() {
    std::unordered_map<std::string, std::string> configValues;

    // open config.ini
    std::ifstream configFile("./config.ini");
    std::string line;

    if (configFile.is_open()) {
        while (std::getline(configFile, line)) { 
            // Read by lines
            size_t delimiterPos = line.find("=");

            if (delimiterPos != std::string::npos) {
                std::string key = line.substr(0, delimiterPos);
                std::string value = line.substr(delimiterPos + 1);

                key.erase(key.find_last_not_of(" \n\r\t") + 1);
                value.erase(0, value.find_first_not_of(" \n\r\t"));

                configValues[key] = value;
            }
        }
        // Close File
        configFile.close();
    } else {
        std::cerr << "Unable to open config.ini" << std::endl;
    }

    return configValues;
}

// open signal
int Signal(int exposure, int dt){
    // Setup stuff:
    wiringPiSetup();

    pinMode(cameraPin, OUTPUT);     // Set camera signal
    pinMode(laserPin, OUTPUT);
    
    std::cout << "Signal is running!\n";
    while(1) {
        // turn on laser
        digitalWrite(laserPin, 1);
        // turn on camera for the first image
        digitalWrite(cameraPin, 1); // Rising edge 
        delay(flash); // g
        digitalWrite(cameraPin, 0); // Falling edge
        delay(dt-flash); // wait delt_t for the second signal
        
        // turn on camera for the second image
        digitalWrite(cameraPin, 1); // Rising edge 
        delay(flash+exposure); // 
        digitalWrite(cameraPin, 0);
        digitalWrite(laserPin, 0);
        delay(1000/freq-flash-dt-exposure); // wait until the next pair
    }
    return 0;
}
    // comments: The problem now is when I put the signal as a function in piv programm, it cannot stop
    // Actually, you need run signal and grab the figures in the same time. We need to redesgin logic.
    // Or maybe we can use two threads. I'm trying to figure this out.
    // BTW I have fixed the reading config function.

int pivGrab(void){
    // The exit code of the sample application.
    int exitCode = 0;

    // Before using any pylon methods, the pylon runtime must be initialized.
    PylonInitialize();

    // Set the number of images to save
    int num_to_save = 200;

    // Initialize Pylon
    Pylon::PylonAutoInitTerm autoInitTerm;

    // Create a device info object and set the device class to a camera
    Pylon::CDeviceInfo info;

    // Create an instant camera object and open it
    CBaslerUniversalInstantCamera camera( CTlFactory::GetInstance().CreateFirstDevice(info) );

    camera.Open();
 
    // Get the camera control object.
    // GenApi::INodeMap& nodemap = camera.GetNodeMap();

    // hardware trigger
    camera.TriggerMode.SetValue("Off");
    //camera.AcquisitionFrameRate.SetValue(20);
    // camera.TriggerSelector.SetValue("FrameStart");
    // camera.TriggerSource.SetValue("Line3");

    // Set the height and width of the camera image
    camera.Height.SetValue(400);
    camera.Width.SetValue(1000);

    // Center the image in the camera's field of view
    camera.CenterX.SetValue(true);
    camera.CenterY.SetValue(true);

    // Get the parameters for setting the image area of interest (Image AOI).
/*
    CIntegerParameter width( nodemap, "Width" );
    CIntegerParameter height( nodemap, "Height" );
    CIntegerParameter offsetX( nodemap, "OffsetX" );
    CIntegerParameter offsetY( nodemap, "OffsetY" );
*/
    // Set the exposure time to 7 ms
    // the max exposure time must be smaller than 20 ms (because of 20 ms time interval for PIV)

    camera.ExposureTimeAbs.SetValue(7000);
    cout << "exposure time = " << camera.ExposureTimeAbs.GetValue()/1000 << " ms" << endl;
    cout << "Width is " << camera.Width.GetValue() << endl;
    cout << "Height is " << camera.Height.GetValue() << endl;
    cout << "Trigger mode is  " << camera.TriggerMode.GetValue() << endl;
    // cout << "Trigger source is " << camera.TriggerSource.GetValue() << endl;

    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d_%H-%M-%S");
    auto mydir = oss.str();
    mkdir(mydir.c_str(), 0777);

    auto tic = std::chrono::high_resolution_clock::now();
    camera.StartGrabbing(num_to_save);
    int i = 0;
    while (camera.IsGrabbing())
    {
        CGrabResultPtr ptrGrabResult;
        // try to get the image
        
            camera.RetrieveResult(2000, ptrGrabResult, TimeoutHandling_ThrowException);
            if (ptrGrabResult->GrabSucceeded())
            {
              CPylonImage image;
              image.AttachGrabResultBuffer(ptrGrabResult);
              std::ostringstream filename;
                filename << "image" << i << ".tif";
                image.Save(ImageFileFormat_Tiff, (mydir + "/" + filename.str()).c_str());
                i++;
            }
            else
                {
                    cout << "Error"<< endl;

                }
        
    }

    camera.Close();

    // Return 0 to indicate successful completion
    return 0;

}


// Main function
int main(int argc, char* argv[])
{
    // Before task -> read config.ini and make sure the signal has open
    std::unordered_map<std::string, std::string> configValues = readConfigFile();

    // GetValue exposure
    if (configValues.count("exposure_time")) {
        int exposure = std::stoi(configValues["exposure_time"]);
        std::cout << "Exposure Time: " << exposure << " seconds" << std::endl;
    } else {
        std::cerr << "Exposure Time not found in config.ini" << std::endl;
    }

    // GetValue dt
    if (configValues.count("dt")) {
        int dt = std::stoi(configValues["dt"]);
        std::cout << "dt: " << dt << " seconds" << std::endl;
    } else {
        std::cerr << "dt not found in config.ini" << std::endl;
    }

    int exposure = std::stoi(configValues["exposure_time"]);
    int dt = std::stoi(configValues["dt"]);

    // Signal(exposure, dt);

    std::thread threadA(Signal(exposure, dt));
    std::thread threadB(pivGrab());

    threadA.join();
    threadB.join();

    return 0;
}
