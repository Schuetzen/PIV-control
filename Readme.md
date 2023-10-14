# Underwater Imaging System project

<p align="center">
  <img src="assets/Raspberry.jpg" alt="Raspberry Pi" width='200'/>
</p>

***This program is an Underwater Imaging System project designed to address various challenges in capturing clear images in underwater environments.***


## Contents
* ⭐ [Features](#features)
* ⭕ [Requirements](#requirements)
* 👉 [Quick Started](#quick-started)
* 🤬[Trouble Shooting](#troubleshooting)


## Features
* Control the laser module for underwater visibility enhancement.
* Interface with the camera for PIV.
* Customize settings for different underwater conditions.
* Use **"make"** commend to build you own project, this file should be found in each programm's folder.
* All variables car be edited in **"config.ini"** file in the same layer of programm.
* After Camera Captured pictures, the laser should be close and delay for 1s.
* The tests fold are used for special situation, and check for hardware's status.
* Check local time

Concept by @[mu-bwang](https://github.com/mu-bwang) 👩🏻‍🔬 | Built with ❤️ by @[Schutzen](https://github.com/Schuetzen)

## Requirements
* Raspberry Pi with Raspbian OS.
* Laser module compatible with Raspberry Pi GPIO.
* Underwater camera compatible with Raspberry Pi.
* Waterproof lines

### ⚠️ Hardware Check ⚠️
* Make sure connection **connects well.**

* Power should at least higher than **18 V.**
 
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

### 1. turn on the laser, run bash commands - Trggier on

```bash
relay on 0

```
 **How to Turn on it manually?**

```bash
sudo screen /dev/nACM0
```

### 2. Camera Setup 
Each time when connect with camera, it should be initialized first.

```bash
# sample
cd ./piv_folder

# build your cpp file
make

# run 
./piv

```

### 3. Other commands

Clean the data folder
```bash
bash ./sample/clear_data.sh

```

Pack the data folder for download
```bash

bash ./sample/pack_data.sh

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
    **This reason might caused by the signal time, if the signal loop break too fast, it will have no trigger signal for the camera.**


## Contributing

If you'd like to contribute to this project, please follow these steps:

1. Fork the repository.

2. Create a new branch for your feature or bug fix.

3. Make your changes and submit a pull request.

4. Provide a detailed description of your changes.

