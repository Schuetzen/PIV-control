#!/bin/bash

# Check if the directory exists
if [ ! -d "../download" ]; then
  echo " There are no '../download' directory."
  exit 1
fi

rm -rf ../download/*

echo "All data has been cleared in download."