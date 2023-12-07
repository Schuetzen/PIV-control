#ifndef PIVGRABFUNCTION_H
#define PIVGRABFUNCTION_H

#include "UtilityFunctions.h"
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <sys/stat.h>
#include <chrono>
#include <pylon/PylonIncludes.h>
#include <pylon/ParameterIncludes.h>
#include <pylon/BaslerUniversalInstantCamera.h>

// Declaration of the pivGrab function.
void* pivGrab(void* arg);

const int RETRIEVE_TIMEOUT = 2000;


#endif // PIVGRABFUNCTION_H
