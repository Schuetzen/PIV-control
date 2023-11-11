# Underwater Imaging System project

<p align="center">
  <img src="assets/Raspberry.jpg" alt="Raspberry Pi" width='200'/>
</p>


***This program is an Underwater Imaging System project designed to address various challenges in capturing images in underwater environments. This program is designed to control a Basler camera and a laser module using the Pylon API and the WiringPi library.*** 

It captures images from the camera and saves them to disk, synchronized with the laser signal. The program is particularly useful for applications requiring precise timing between image captures and external events, such as in Particle Image Velocimetry (PIV) systems.

`last_edited_time`: 11092023


## Contents
* ⭐ [Features](#features)
* ⭕ [Requirements](#requirements)
* 👉 [Quick Started](#quick-started)
* 🤬 [Trouble Shooting](#troubleshooting)
* 🤖 [Techlogical Configuration](#backup-tech-configuration)!

<p align="center">
  <img src="assets/image.png" alt="Raspberry Pi" width='750'/>
</p>




## Features
- Control of Basler camera using the Pylon API.
- Laser signal generation using the WiringPi library.
- Synchronization of image capture with laser pulses.
- Configuration of camera settings through a `config.ini` file.
- Saving captured images in TIFF format.
- Logging of operations with timestamps.

Concept by @[mu-bwang](https://github.com/mu-bwang) 👩🏻‍🔬 | Built with ❤️ by @[Schutzen](https://github.com/Schuetzen)

## Requirements
- A Basler camera compatible with the Pylon API.
- A Raspberry Pi or similar device for GPIO control (if using laser synchronization).
- Pylon SDK installed on the system.
- WiringPi library (for Raspberry Pi users).
- C++ development environment with support for C++11 or later.

## Installation
1. **Install the Pylon SDK**: Download and install the Pylon SDK from the Basler website.
2. **Install WiringPi** (Raspberry Pi users): Run `sudo apt-get install wiringpi`.
3. **Clone the Repository**: Clone this repository to your local machine.
```bash 
git clone [repository URL]
```

4. **Build the Program**: Navigate to the program directory and compile the code.
```bash
cd [program directory]
g++ -o camera_control main.cpp -lpylon -lwiringPi -lpthread
```
### ⚠️ Software Check ⚠️
- Ensure that the Pylon SDK is correctly installed and configured on your system.
- For Raspberry Pi users, make sure WiringPi is installed and the GPIO pins are correctly configured.
- The program is designed for systems with C++11 support. Ensure your compiler is compatible.
- Modify the thread functions `Signal` and `pivGrab` as per your specific hardware setup and requirements.


### ⚠️ Hardware Check ⚠️
* Make sure connection **connects well.**

* Power should at least higher than **18 V.** (In order to make laser run) 

* **22 V** would be the best.

* The line that has been connect has the voltage **12 V**.

* The Camera(Basler) can only set delta t higher than 9.1 ms.

* More Information please check the [Hardware Configuration](#hardware-configuration)
 
### 🔗 Requirement library

This code is a C++ program that leverages various libraries and headers to facilitate a range of functionalities. Here's a detailed breakdown of each component:

1. **Pylon Library**:
    
    * `#include <pylon/PylonIncludes.h>`: This includes the essential header files from the Pylon library. Pylon is a comprehensive set of tools and libraries used for acquiring, processing, and displaying images from different types of cameras. The included files likely contain functions and classes necessary for camera interaction.
        
    * `#include <pylon/ParameterIncludes.h>`: This includes header files that handle camera parameters. This might encompass settings like exposure, gain, and other configurations that can be adjusted on the camera.
        
    * `#include <pylon/BaslerUniversalInstantCamera.h>`: This includes specific headers tailored for Basler cameras. Basler is a prominent brand of industrial cameras known for their high-quality imaging capabilities.
        
2. **WiringPi Library**:
    
    * `#include <wiringPi.h>`: This includes the WiringPi library, which is a set of functions designed for working with the GPIO (General Purpose Input/Output) pins on a Raspberry Pi. In this context, it is likely being used for handling signals, possibly related to hardware interactions.
3. **Standard C++ Libraries**:
    
    * `#include <iostream>`: This provides access to the standard input/output stream functionalities. It's used for reading and writing data through the console.
        
    * `#include <ctime>`: This includes functions and types related to date and time operations.
        
    * `#include <chrono>`: This introduces elements from the C++ standard library for handling time.
        
    * `#include <sstream>`: This header supports string stream processing, allowing operations on strings as if they were input/output streams.
        
    * `#include <iomanip>`: This provides tools for manipulating stream input and output. It's often used for formatting.
        
    * `#include <cstdio>`: This is part of the C standard input/output library, offering basic input and output functions.
        
    * `#include <cstdlib>`: This includes general utilities functions and macros.
        
    * `#include <cstring>`: This provides functionality for handling C-style strings.
        
    * `#include <sys/stat.h>` and `#include <sys/types.h>`: These headers are for working with file information and file system attributes.
        
    * `#include <string>`: This includes the standard C++ string class, allowing for the manipulation and handling of strings.
        
    * `#include <unistd.h>`: This includes various standard symbolic constants and types. It often deals with system calls.
        
4. **File Operations**:
    
    * `#include <fstream>`: This brings in the necessary tools for performing file input and output operations.
        
    * `#include <unordered_map>`: This includes the header for an unordered associative container, which maps unique keys to values. This can be very useful for efficient data retrieval based on unique identifiers.
        
5. **Multithreading**:
    
    * `#include <pthread.h>`: This header belongs to the POSIX threads library, which offers functions for creating and managing threads. This enables the program to execute multiple tasks concurrently, potentially improving performance and responsiveness.


## Quick Started

### 1. Capture pcitures 
Each time when connect with camera, it should be initialized first.

```bash
# go to the main code
cd ./cam_cpp/build

# run codes
./piv
```

### 2. Edit source code

If you need to edit the script,
```bash
cd ./cam_cpp/source
```
`piv.c` is the target source script.

If you need to compile it, type
```bash
make
```
It will create the target obejct file and executable file in foler `build`
### 3. Other commands

Trun on laser
```bash
cd ./cam_cpp/build
./laser_on
```

Trun off laser
```bash
cd ./cam_cpp/build
./laser_off
```

Clean the data folder
```bash
cd ./cam_cpp
bash ./commands/clean_data.sh
```

Pack the data folder for download
```bash
cd ./cam_cpp
bash ./commands/pack_data.sh
```


## Configuration

the config.ini sample can be written as:
**Before you edit the config file, make sure the parameter's name are correct.**

```ini
; Sample Test Case
exposure_time_in_ms = 7
dt_in_ms = 10 
Frequency = 10
Duration_in_sec = 20
Height = 400
Width = 1000

; delta t should higher than 9
; The following parameters are used to set the camera's parameters.
; The parameters are set using the following formula:
; num_cameras = Frequency * Duration_in_sec * 2

```
## Troubleshooting

Here are the problems happend normally.

*  **The laser's light is low**
    Check the power first

* **Error: Timeout Exception**

    `terminate called after throwing an instance of 'GenICam_3_1_Basler_pylon::TimeoutException'
Aborted` 
    **This reason might caused by the camera buffer, this might happen in the first time running codes**


## Backup Tech-Configuration

### Hardware Configuration
The following section delineates the hardware configuration for the Raspberry Pi 4 setup, focusing on the GPIO pin assignments.

<p align="center">
  <img src="assets/Raspberry-Pi-GPIO.png" alt="Raspberry GPIO" width='400'/>
</p>


*GPIO Pin Assignments*

Utilizing the WiringPi library, specific pins are designated for various components in this setup:

* Laser and Camera Connection:

**GPIO 17 (Pin 11)** - **Camera unit**
**GPIO 27 (Pin 13)** - **Laser unit**

* Real-Time Clock (RTC) Module Connection:

**GPIO 2 SDA (Pin3)** - **RTC**
**GPIO 3 SCL (Pin5)** - **RTC**

<p align="center">
  <img src="assets/Pin-diagram.png" alt="Raspberry GPIO" width='800'/>
</p>


### Software Architecture

This section elucidates the data flow among different directories and outlines the purpose each directory serves in this system.

* `data`: This directory is intended for storing image files that are used or generated by the application. It is the primary location for all picture-related data.
    
* `assets`: This directory contains media files that are utilized in the `Readme.md` for documentation purposes. Typically, it includes images that are embedded within this document to illustrate concepts or to provide examples.
    
* `build`: Here, you will find all compiled files. This includes `piv.o`, which is an object file, and the final executable named `piv`. This is where the build process outputs its results.
    
* `download`: This directory is used for packaging. It compiles folders and files from the `data` directory into a distributable format, making it easier to manage versioning and distribution.
    
* `tests`: Contains scripts and commands for running tests. These bash scripts are used to validate the functionality of the application through automated testing procedures.
    
* `sample`: In this directory, sample test files are stored. These are used to demonstrate the capabilities of the application or to provide templates for users to follow.
    
* `source`: The main source files of the application are located here. It is the heart of the project, containing the primary logic, algorithms, and functionality.
    
* `scripts`: Additional scripts related to camera control and other auxiliary functions can be found here. These are not the main executable scripts but support the application, often interfacing with external hardware or services.


## Contributing

If you'd like to contribute to this project, please follow these steps:

1. Fork the repository.

2. Create a new branch for your feature or bug fix.

3. Make your changes and submit a pull request.

4. Provide a detailed description of your changes.

## License

BSD 2-Clause License

Copyright (c) 2023, Schützen Jung (應旭臣)

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.