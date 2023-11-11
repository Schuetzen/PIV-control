#!/bin/bash

# Set the system time to October 1st, 2023
sudo date +%Y%m%d -s "20231001"

# Set the system time to 00:00:00
sudo date +%T -s "00:00:00"

# Sync the hardware clock with the system time
sudo hwclock --systohc

echo "The system time has been set to October 1st, 2023."
