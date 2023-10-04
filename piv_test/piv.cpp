//piv.cpp
/*
    Note: This code is modified from the example code provided by Basler.
    The program uses the Pylon SDK package to interface with the camera. 
    The program starts by initializing the PylonAutoInitTerm object, 
    which is responsible for initializing and terminating the Pylon runtime system. 
    The CDeviceInfo object is then created and set to the device class of the camera. 
    An instance of the CInstantCamera object is created using the CreateFirstDevice method of the CTlFactory object, 
    which returns the first available device matching the specified device class.

*/

// Include necessary headers (to use Pylon API)
#include <pylon/PylonIncludes.h>
#include <pylon/ParameterIncludes.h>
#include <pylon/BaslerUniversalInstantCamera.h>

// Include standard headers
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
#include <fstream>


// Namespace for using pylon objects.
using namespace Pylon;

// Namespace for using cout.
using namespace std;

// Namespace for using pylon universal instant camera parameters.
using namespace Basler_UniversalCameraParams;

// Forward declarations for helper functions
bool IsColorCamera( CBaslerUniversalInstantCamera& camera );
void AutoGainOnce( CBaslerUniversalInstantCamera& camera );
void AutoGainContinuous( CBaslerUniversalInstantCamera& camera );
void AutoExposureOnce( CBaslerUniversalInstantCamera& camera );
void AutoExposureContinuous( CBaslerUniversalInstantCamera& camera );
void AutoWhiteBalance( CBaslerUniversalInstantCamera& camera );

/**
 * @brief Main function that acquires images from a camera and saves them as TIFF files.
 * 
 * @param argc Number of command line arguments.
 * @param argv Array of command line arguments.
 * @return int Exit code.
 */
int main(int argc, char* argv[])
{

    // Before using any pylon methods, the pylon runtime must be initialized.
    // PylonInitialize();

    // numeber of figures that saved in folder
    int num_to_save = 200;

    Pylon::PylonAutoInitTerm autoInitTerm;
    Pylon::CDeviceInfo info;

    
    // info.SetDeviceClass(Camera_t::DeviceClass());
    // Create an instant camera object with the first found camera device.
    CBaslerUniversalInstantCamera camera( CTlFactory::GetInstance().CreateFirstDevice(info) );

    // CInstantCamera camera(CTlFactory::GetInstance().CreateFirstDevice(info));
    
    // Print the model name of the camera.
    cout << "Using device " << camera.GetDeviceInfo().GetModelName() << endl;

    try
    {
        
        camera.Height.SetValue(400);
        camera.Width.SetValue(1000);
        camera.CenterX.SetValue(true);
        camera.CenterY.SetValue(true);
        // the max exposure time must be smaller than 20 ms (because of 20 ms time interval for PIV)
        camera.ExposureTime.SetValue(7000);
        cout << "exposure time = " << camera.ExposureTimeAbs.GetValue()/1000.0 << " ms" << endl;
        camera.MaxNumBuffer.SetValue(1000);
        // ------------------
        // hardware trigger
        camera.TriggerMode.SetValue("On");
        camera.TriggerSelector.SetValue("FrameStart");
        camera.TriggerSource.SetValue("Line3");
        
        camera.Open();
        // ------------------
        // software trigger
        // camera.TriggerSource.SetValue("Software");
        // camera.AcquisitionFrameRateAbs.SetValue(20);
        // camera.AcquisitionFrameRateEnable();
        // cout << camera.AcquisitionFrameRateAbs.GetValue() << endl;
        // camera.AcquisitionMode.SetValue("Continuous");

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
            try
            {
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
                    cout << "Error: " << ptrGrabResult->GetErrorCode() << " " << ptrGrabResult->GetErrorDescription() << endl;
                    ofstream logFile;
                    logFile.open("error.log", ios::app);
                    logFile << "Error: " << ptrGrabResult->GetErrorCode() << " " << ptrGrabResult->GetErrorDescription() << endl;
                    logFile.close();
                }
            }
            // catch timeout exception
            catch (const GenericException &e)
            {
                cout << "Error: " << e.GetDescription() << endl;
                ofstream logFile;
                logFile.open("error.log", ios::app);
                logFile << "Error: " << e.GetDescription() << endl;
                logFile.close();
            }
        }
        camera.StopGrabbing();
        camera.Close();
        auto toc = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = toc - tic;
        cout << "Image acquisition done! Time used = " << elapsed.count() << " s" << endl;
    }
    // catch timeout exception
    catch (const GenericException &e)
    {
        cout << "Error: " << e.GetDescription() << endl;
        ofstream logFile;
        logFile.open("error.log", ios::app);
        logFile << "Error: " << e.GetDescription() << endl;
        logFile.close();
    }
    return 0;
}