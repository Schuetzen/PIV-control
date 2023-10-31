#!/bin/bash

# Check if the directory exists
if [ ! -d "../data" ]; then
  echo " There are no '../data' directory."
  exit 1
fi

rm -rf ../data/*

echo "All data has been cleared in data."