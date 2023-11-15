// Include necessary headers (to use Pylon API)
#include <pylon/PylonIncludes.h>
#include <pylon/ParameterIncludes.h>
#include <pylon/BaslerUniversalInstantCamera.h>

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

// Use the Pylon namespace and the standard namespace
using namespace Pylon;
using namespace std;

// Main function
int main(int argc, char* argv[])
{
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