# Underwater Imaging System project


## Contents
* ⭐ [Features](##feature)
* ⭕ [Requirements](##Requirements)
* 👉 [Quick Started](##QuickStarted)


## Features
* Control the laser module for underwater visibility enhancement.
* Interface with the camera for PIV.
* Customize settings for different underwater conditions.
* Use **"make"** commend to build you own project, this file should be found in each programm's folder.
* All variables car be edited in **"config.ini"** file in the same layer of programm.
* After Camera Captured pictures, the laser should be close and delay for 1s.
* The tests fold are used for special situation, and check for hardware's status.

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
* 

## Quick Started

### 1. turn on the laser, run bash commands - Trggier on

```
relay on 0

```
 **How to Turn on it manually?**

```bash
sudo screen /dev/nACM0
```

### 2. camera set up 
Each time when connect with camera, it should be initialized first.

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

## Errors Guild
Here are the problems happend normally.

*  **The laser's light is low**
    Check the power first



## Contributing

If you'd like to contribute to this project, please follow these steps:

1. Fork the repository.

2. Create a new branch for your feature or bug fix.

3. Make your changes and submit a pull request.

4. Provide a detailed description of your changes.

