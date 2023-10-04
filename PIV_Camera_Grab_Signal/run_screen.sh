#!/bin/bash

# Open a screen session to /dev/ttyACM0
screen /dev/ttyACM0

# Send an "Enter" key press
echo -ne '\n'
