# Underwater Laser and Camera Control Program


## Contents
* ⭐ [Features](##Features)
* ⭕ [Requirements](##Requirements)
* 👉 [Quick Started](##QuickStarted)


## Features
* Control the laser module for underwater visibility enhancement.
* Interface with the camera for PIV.
* Customize settings for different underwater conditions.
* Easy integration with Raspberry Pi.

## Requirements
* Raspberry Pi with Raspbian OS.
* Laser module compatible with Raspberry Pi GPIO.
* Underwater camera compatible with Raspberry Pi.
* Waterproof lines

Compile trigger program: (signal)
gcc -o signal signal.cpp -l wiringPi

#####################################

To enable auto run, open bashrc file using:

---
nano ~/.bashrc
---

uncomment the following commands:

cd Desktop/cam_python 
sleep 10
./signal &
python3 piv.py &
## Open the Laser

```
sudo screen /dev/nACM0
```

## PIV Program
g++ -o piv piv.cpp -l wiringPi

## Shut off signal
g++ -o shut_off_signal shut_off_signal.cpp -l wiringPi

## Quick Started

## Configuration

## Contributing
If you'd like to contribute to this project, please follow these steps:

1. Fork the repository.

2. Create a new branch for your feature or bug fix.

3. Make your changes and submit a pull request.

4. Provide a detailed description of your changes.

