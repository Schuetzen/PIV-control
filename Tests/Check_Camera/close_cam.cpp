#include <pylon/PylonIncludes.h>
#include <iostream>

using namespace Pylon;

int main()
{
    PylonInitialize();
    try
    {
        CInstantCamera camera(CTlFactory::GetInstance().CreateFirstDevice());
        camera.StopGrabbing();
        camera.Close();
    }
    catch (const GenericException &e)
    {
        std::cerr << "An exception occurred: " << e.GetDescription() << std::endl;
        exitCode = 1;
    }
    PylonTerminate();
    return 0;
}
