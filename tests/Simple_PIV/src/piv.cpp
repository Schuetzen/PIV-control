/*
C++ program that captures images from a camera and saves them to disk. 
The program uses the Pylon API to interact with the camera and the WiringPi library to control a laser and camera signal.
The program also reads configuration values from a file named config.ini using the readConfigFile function.
*/

// Include necessary headers for the Pylon API to interact with Basler cameras.
#include <pylon/PylonIncludes.h>
#include <pylon/ParameterIncludes.h>
#include <pylon/BaslerUniversalInstantCamera.h>

// Include the WiringPi library for GPIO control on Raspberry Pi.
#include <wiringPi.h>

// Standard C++ library headers for various functionalities.
#include <iostream> // For input and output operations.
#include <ctime>    // For time-related functions.
#include <chrono>   // For high-resolution clock and timing operations.
#include <sstream>  // For string stream operations, useful in formatting strings.
#include <iomanip>  // For parametric manipulators, used to format output.
#include <cstdio>   // For C-style input-output (e.g., printf).
#include <cstdlib>  // For general-purpose functions such as system calls.
#include <cstring>  // For string manipulation functions.
#include <sys/stat.h> // For querying file system status.
#include <sys/types.h> // For defining various data types used in system calls.
#include <string>   // For using the string class.
#include <unistd.h> // For POSIX operating system API.
#include <fstream>  // For file stream operations (e.g., reading from files).
#include <unordered_map> // For using hash tables (unordered maps).
#include <pthread.h> // For using POSIX threads.

#include <./UtilityFunctions.h>


// Use the Pylon namespace to avoid prefixing Pylon functions with their namespace.
using namespace Pylon;
// Use the standard namespace to avoid prefixing standard functions with 'std::'.
using namespace std;

const int FLASH_DURATION = 1; // Duration of the flash signal in milliseconds
const int RETRIEVE_TIMEOUT = 2000; // Timeout for camera image retrieval in milliseconds


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
    
    int ext = dur / 10;

    // verify the maximal time interval allowed. 50Hz->20ms
    // const int dt = 10;  

    int flash = FLASH_DURATION;
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

    PrintCurrentTimeAndMessage("Laser is running!");    
    auto start_time = std::chrono::steady_clock::now();
    while(true) {

        // exit loop after dur seconds
        auto current_time = std::chrono::steady_clock::now();
        auto elapsed_time = std::chrono::duration_cast<std::chrono::seconds>(current_time - start_time).count();
        if (elapsed_time >= dur + ext) {
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
    int dt = params[5];

    // Before using any pylon methods, the pylon runtime must be initialized.
    PylonInitialize();
    PrintCurrentTimeAndMessage("Pylon Cameara initialized successfully.");

    // Set the number of images to save 
    // duration mutiple frequency mutiple two, two picture
    int num_to_save = dur*freq*2;

    // Initialize Pylon
    Pylon::PylonAutoInitTerm autoInitTerm;

    // Create a device info object and set the device class to a camera
    Pylon::CDeviceInfo info;

    PrintCurrentTimeAndMessage("Waiting for Camera");

    // sleep(5);    

    // Create an instant camera object and open it
    CBaslerUniversalInstantCamera camera( CTlFactory::GetInstance().CreateFirstDevice(info) );


    camera.Open();
    
    PrintCurrentTimeAndMessage("Camera Opend Successfully");

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

    camera.ExposureTimeAbs.SetValue(exposure*1000);

    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d_%H-%M");

    auto mydir = oss.str();

    std::string dataDir = "../data/" + mydir;
    // std::string dataDir = mydir;

    mkdir(dataDir.c_str(), 0777);
    PrintCurrentTimeAndMessage("Data saved in the " + dataDir);

    // Open a log file in the same directory where the images will be saved
    std::ofstream logFile(dataDir + "/Readme.txt");
    if (!logFile.is_open()) {
    std::cerr << "Failed to create log file" << std::endl;
    return nullptr; // or handle the error as you see fit
    }

    // Redirect cout to log file
    std::streambuf* coutbuf = std::cout.rdbuf(); //save old buf
    std::cout.rdbuf(logFile.rdbuf()); //redirect std::cout to log.txt!

    // save in the Readme file
    std::cout << "Exposure time = " << camera.ExposureTimeAbs.GetValue()/1000 << " ms" << std::endl;
    std::cout << "Delta t = " << dt << " ms"<< std::endl;
    std::cout << "Duration time = " << dur << " s"<< std::endl;
    std::cout << "Width = " << camera.Width.GetValue() << " pix"<< std::endl;
    std::cout << "Height = " << camera.Height.GetValue() << " pix"<< std::endl;
    std::cout << "Trigger mode is  " << camera.TriggerMode.GetValue() << std::endl;
    std::cout << "Expected Picture number is " << num_to_save << std::endl;

    auto tic = std::chrono::high_resolution_clock::now();
    camera.StartGrabbing(num_to_save);
    int i = 0;
    while (camera.IsGrabbing())
    {
        CGrabResultPtr ptrGrabResult;
        // try to get the image
        try
        {
            camera.RetrieveResult(RETRIEVE_TIMEOUT, ptrGrabResult, TimeoutHandling_ThrowException);
        if (ptrGrabResult->GrabSucceeded())
        {
            CPylonImage image;
            image.AttachGrabResultBuffer(ptrGrabResult);
            std::ostringstream filename;
            filename << "image" << std::setfill('0') << std::setw(7) << i << ".tif";
            // image.Save(ImageFileFormat_Tiff, (mydir + "/" + filename.str()).c_str());
            image.Save(ImageFileFormat_Tiff, (dataDir + "/" + filename.str()).c_str());
            i++;
        }
        else
        {
            std::cout << "Error" << std::endl;
        }
        }
        catch(const std::exception& e)
        {
            std::cerr << "Exception occurred: " << e.what() << std::endl;
            return (void*)EXIT_FAILURE;
        }
        
        
    }

    camera.Close();

    PrintCurrentTimeAndMessage("Camera Closed Successfully");

    // Redirect cout back to its original buffer
    std::cout.rdbuf(coutbuf); //reset to standard output again

    logFile.close();

    // Return 0 to indicate successful completion
    return nullptr;

}

// The main function where the program starts executing.
int main(int argc, char* argv[])
{
    // Obtain the current time as a time_t object
    time_t now = time(0);

    // Convert it to local time
    tm *ltm = localtime(&now);

    PrintCurrentTimeAndMessage("Code is running, please wait ...");

    // Create a timestamp string for the filename
    std::ostringstream oss;
    oss << "../logs/log_" << 1900 + ltm->tm_year << "-" 
        << std::setfill('0') << std::setw(2) << 1 + ltm->tm_mon << "-" 
        << std::setfill('0') << std::setw(2) << ltm->tm_mday << "_" 
        << std::setfill('0') << std::setw(2) << ltm->tm_hour << "-" 
        << std::setfill('0') << std::setw(2) << ltm->tm_min << "-" 
        << std::setfill('0') << std::setw(2) << ltm->tm_sec << ".txt";
    std::string logFileName = oss.str();

    // Open a log file with the timestamped name
    std::ofstream logFile(logFileName);
    if (!logFile.is_open()) {
        std::cerr << "Failed to create log file: " << logFileName << std::endl;
        return 1; // Exit if cannot open log file
    }

    // Redirect cout to log file
    std::streambuf* coutbuf = std::cout.rdbuf(); // Save old buffer
    std::cout.rdbuf(logFile.rdbuf()); // Redirect std::cout to the log file


    // Print head
    std::cout << "****************************************" << std::endl;
    std::cout << "Log @ ";
    // Output the time in MM:DD:hh:mm format
    std::cout << std::setfill('0') << std::setw(2) << ltm->tm_mon + 1 << ":"; // Months are 0-11, so add 1
    std::cout << std::setfill('0') << std::setw(2) << ltm->tm_mday << ":";
    std::cout << std::setfill('0') << std::setw(2) << ltm->tm_hour << ":";
    std::cout << std::setfill('0') << std::setw(2) << ltm->tm_min << std::endl;
    std::cout << "****************************************" << std::endl;

    // Before task -> read config.ini and make sure the signal has open
    std::unordered_map<std::string, std::string> configValues = readConfigFile();

    PrintCurrentTimeAndMessage("Reading config.ini");

    // GetValue 
    int exposure = std::stoi(configValues["exposure_time_in_ms"]);
    int dt = std::stoi(configValues["dt_in_ms"]);
    int freq = std::stoi(configValues["Frequency"]);
    int dur = std::stoi(configValues["Duration_in_sec"]);
    int height = std::stoi(configValues["Height"]);
    int width = std::stoi(configValues["Width"]);

    // Print configuration values in a structured manner
    std::cout << "******** Configuration Settings ********" << std::endl;
    std::cout << "Delta Time (dt): " << dt << " ms" << std::endl;
    std::cout << "Exposure Time: " << exposure << " ms" << std::endl;
    std::cout << "Frequency: " << freq << " Hz" << std::endl; 
    std::cout << "Duration: " << dur << " seconds" << std::endl;
    std::cout << "Image Height: " << height << " pixels" << std::endl;
    std::cout << "Image Width: " << width << " pixels" << std::endl;
    std::cout << "***************************************" << std::endl;

    // Create two threads, one for signal, one for grab, make sure they are running at the same time
    pthread_t threadA, threadB;

    // Create new threads;
    int params_signal[4] = {exposure, dur, freq, dt};
    
    int params_piv[6] = {exposure, dur, freq, height, width, dt};

    PrintCurrentTimeAndMessage("Laser start");

    pthread_create(&threadA, nullptr, Signal, (void*) params_signal);

    PrintCurrentTimeAndMessage("Camera start");

    pthread_create(&threadB, nullptr, pivGrab, (void*) params_piv);

    // wait till thread end
    pthread_join(threadA, nullptr);
    pthread_join(threadB, nullptr);

    return 0;
}