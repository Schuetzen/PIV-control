#include "pivGrabFunction.h"


using namespace Pylon;
using namespace std;


// Implementation of pivGrab function
void* pivGrab(void* arg) {
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
