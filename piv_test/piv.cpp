/*
C++ program that captures images from a camera and saves them to disk. 
The program uses the Pylon API to interact with the camera and the WiringPi library to control a laser and camera signal.
The program also reads configuration values from a file named config.ini using the readConfigFile function.
*/

// Include necessary headers (to use Pylon API)
#include <pylon/PylonIncludes.h>
#include <pylon/ParameterIncludes.h>
#include <pylon/BaslerUniversalInstantCamera.h>

// Include WiringPi library! -> for signal
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
#include <string>
#include <unistd.h>

// Include for config read
#include <fstream>
#include <unordered_map>

// Include for thread
#include <pthread.h>

// Use the Pylon namespace and the standard namespace
using namespace Pylon;
using namespace std;


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


/**
 * This function generates a signal for a PIV camera to capture two images with a certain time delay between them.
 * @param arg A pointer to an integer array containing the following parameters:
 *            - exposure: an integer representing the exposure time of the camera in milliseconds.
 *            - dt: an integer representing the time delay between two consecutive image captures in milliseconds.
 *            - freq: an integer representing the frequency of image captures in Hz.
 * @return A null pointer.
 */
void* Signal(void* arg){
    // params
    int* params = (int*) arg;
    int exposure = params[0];
    int dur = params[1];
    int freq = params[2];
    int dt = params[3];
    
    // verify the maximal time interval allowed. 50Hz->20ms
    // const int dt = 10;  

    int flash = 1;
    // int freq = 10;
    // 10 ms of camera exposure time
    //const int exposure = 7; 
    // int exposure;

    // Setup stuff:
    wiringPiSetup();

    //wiringPi pin 0 = BCM GPIO pin 17 = physical pin 11
    int cameraPin = 0;    
    // wiringPi pin 2 = BCM GPIO pin 27 = physical pin 13 
    int laserPin = 2;    

    pinMode(cameraPin, OUTPUT);     // Set camera signal
    pinMode(laserPin, OUTPUT);
    
    std::cout << "Signal is running!\n";
    auto start_time = std::chrono::steady_clock::now();
    while(true) {

        // exit loop after dur seconds
        auto current_time = std::chrono::steady_clock::now();
        auto elapsed_time = std::chrono::duration_cast<std::chrono::seconds>(current_time - start_time).count();
        if (elapsed_time >= dur) {
            break;
        }
        
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
    return nullptr;
}


/**
 * This function grabs images from a Basler camera using Pylon API and saves them as TIFF files.
 * 
 * @param arg An integer array containing the following parameters in order:
 *            - exposure: the exposure time in milliseconds
 *            - dur: the duration of the image acquisition in seconds
 *            - freq: the frequency of image acquisition in Hz
 *            - height: the height of the camera image in pixels
 *            - width: the width of the camera image in pixels
 * 
 * @return void* A null pointer indicating successful completion of the function.
 */
void* pivGrab(void* arg){
    // The exit code of the sample application.
    int exitCode = 0;

    // params
    int* params = (int*) arg;
    int exposure = params[0];
    int dur = params[1];
    int freq = params[2];
    int height = params[3];
    int width = params[4];

    // Before using any pylon methods, the pylon runtime must be initialized.
    PylonInitialize();
    std::cout<<"Pylon Cameara initialized successfully."<<std::endl;

    // Set the number of images to save 
    // duration mutiple frequency mutiple two, two picture
    int num_to_save = dur*freq*2;

    // Initialize Pylon
    Pylon::PylonAutoInitTerm autoInitTerm;

    // Create a device info object and set the device class to a camera
    Pylon::CDeviceInfo info;

    std::cout<<"Waiting for Camera"<<std::endl;

    // sleep(5);    

    // Create an instant camera object and open it
    CBaslerUniversalInstantCamera camera( CTlFactory::GetInstance().CreateFirstDevice(info) );


    camera.Open();
 
    // Get the camera control object.
    // GenApi::INodeMap& nodemap = camera.GetNodeMap();

    // hardware trigger
    camera.TriggerMode.SetValue("On");
    //camera.AcquisitionFrameRate.SetValue(20);
    camera.TriggerSelector.SetValue("FrameStart");
    camera.TriggerSource.SetValue("Line3");

    // Set the height and width of the camera image
    camera.Height.SetValue(height);
    camera.Width.SetValue(width);

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

    camera.ExposureTimeAbs.SetValue(exposure*1000);
    std::cout << "exposure time = " << camera.ExposureTimeAbs.GetValue()/1000 << " ms" << std::endl;
    std::cout << "Width is " << camera.Width.GetValue() << std::endl;
    std::cout << "Height is " << camera.Height.GetValue() << std::endl;
    std::cout << "Trigger mode is  " << camera.TriggerMode.GetValue() << std::endl;

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
            filename << "image" << std::setfill('0') << std::setw(7) << i << ".tif";
            image.Save(ImageFileFormat_Tiff, (mydir + "/" + filename.str()).c_str());
            i++;
        }
        else
        {
            std::cout << "Error" << std::endl;
        }
    }

    camera.Close();

    // Return 0 to indicate successful completion
    return nullptr;

}




// Main function
int main(int argc, char* argv[])
{
    // Before task -> read config.ini and make sure the signal has open
    std::unordered_map<std::string, std::string> configValues = readConfigFile();
    std::cout<<"Read config file"<<std::endl;
    
    /*
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
    */

    // GetValue 
    int exposure = std::stoi(configValues["exposure_time_in_ms"]);
    int dt = std::stoi(configValues["dt_in_ms"]);
    int freq = std::stoi(configValues["Frequency"]);
    int dur = std::stoi(configValues["Duration_in_sec"]);
    int height = std::stoi(configValues["Height"]);
    int width = std::stoi(configValues["Width"]);


    // Create two threads, one for signal, one for grab, make sure they are running at the same time
    pthread_t threadA, threadB;

    // Create new threads;
    int params_signal[4] = {exposure, dur, freq, dt};
    
    int params_piv[5] = {exposure, dur, freq, height, width};

    std::cout<<"start signal"<<std::endl;

    pthread_create(&threadA, nullptr, Signal, (void*) params_signal);

    std::cout<<"start camera"<<std::endl;

    pthread_create(&threadB, nullptr, pivGrab, (void*) params_piv);
    
    

    // std::thread threadA(Signal(exposure, dt));
    // std::thread threadB(pivGrab());

    // wait till thread end
    pthread_join(threadA, nullptr);
    pthread_join(threadB, nullptr);

    return 0;
}