#!/bin/bash

# Check if the directory exists
if [ ! -d "../data" ]; then
  echo " There are no '../data' directory."
  exit 1
fi

rm -rf ../data/*

# Check if the directory exists
if [ ! -d "../logs" ]; then
  echo " There are no '../logs' directory."
  exit 1
fi

rm -rf ../logs/*

echo "All data has been cleared in data."
echo "All data has been cleared in logs."