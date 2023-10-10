// Include necessary headers
#include <pylon/PylonIncludes.h>
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
    // Set the number of images to save
    int num_to_save = 200;

    // Initialize Pylon
    Pylon::PylonAutoInitTerm autoInitTerm;

    // Create a device info object and set the device class to a camera
    Pylon::CDeviceInfo info;
    info.SetDeviceClass(Camera_t::DeviceClass());

    // Create an instant camera object and open it
    CInstantCamera camera(CTlFactory::GetInstance().CreateFirstDevice(info));
    camera.Open();

    // Set the height and width of the camera image
    camera.Height.SetValue(400);
    camera.Width.SetValue(1000);

    // Center the image in the camera's field of view
    camera.CenterX.SetValue(true);
    camera.CenterY.SetValue(true);

    // Set the exposure time to 7 ms
    // the max exposure time must be smaller than 20 ms (because of 20 ms time interval for PIV)

    camera.ExposureTimeAbs.SetValue(7000);
    cout << "exposure time = " << camera.ExposureTimeAbs.GetValue()/1000 << " ms" << endl;

    // Set the maximum number of buffers to 1000
    camera.MaxNumBuffer.SetValue(1000);

    // Set up the hardware trigger
    camera.TriggerMode.SetValue("On");
    camera.TriggerSelector.SetValue("FrameStart");
    camera.TriggerSource.SetValue("Line3");

    // Set up the software trigger (commented out)
    // camera.TriggerSource.SetValue("Software");
    // camera.AcquisitionFrameRateAbs.SetValue(20);
    // camera.AcquisitionFrameRateEnable();
    // cout << camera.AcquisitionFrameRateAbs.GetValue() << endl;
    // camera.AcquisitionMode.SetValue("Continuous");

    // Create a directory with the current date and time as its name
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d_%H-%M-%S");
    auto mydir = oss.str();
    mkdir(mydir.c_str(), 0777);

    // Start grabbing images and save them to disk
    auto tic = std::chrono::high_resolution_clock::now();
    camera.StartGrabbingMax(num_to_save);
    int i = 0;
    while (camera.IsGrabbing())
    {
        CGrabResultPtr ptrGrabResult;
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
        }
    }
    camera.StopGrabbing();
    camera.Close();
    auto toc = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = toc - tic;
    cout << "Image acquisition done! Time used = " << elapsed.count() << " s" << endl;

    // Return 0 to indicate successful completion
    return 0;
}