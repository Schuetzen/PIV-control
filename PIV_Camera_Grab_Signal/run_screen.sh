#!/bin/bash

# Open a screen session to /dev/ttyACM0
sudo screen /dev/ttyACM0

# Send an "Enter" key press
echo -ne '\n'
