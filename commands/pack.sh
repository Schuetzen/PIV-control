#!/bin/bash

# Define the directories relative to the current one
downloads_dir="../download"
data_dir="../data"

# Clear all files in the ../downloads directory
echo "Clearing all files in $downloads_dir..."
rm -rf "$downloads_dir"/*

# Check if the rm was successful
if [ $? -ne 0 ]; then
    echo "Failed to clear $downloads_dir"
    exit 1
fi

# Create the ../downloads directory if it doesn't exist
if [ ! -d "$downloads_dir" ]; then
    mkdir -p "$downloads_dir"
fi

# Iterate over each folder in the ../data directory
for folder in "$data_dir"/*/; do
    # Check if the variable is a directory
    if [ -d "$folder" ]; then
        # Get the name of the folder without the path
        folder_name=$(basename "$folder")
        
        # Create a tar.gz package for each folder
        echo "Packing $folder_name into a tar.gz archive..."
        tar -czf "$downloads_dir/${folder_name}.tar.gz" -C "$data_dir" "$folder_name"
        
        # Check if the tar command was successful
        if [ $? -ne 0 ]; then
            echo "Failed to pack $folder_name"
            exit 1
        fi
    fi
done

echo "All folders have been packed into archives in $downloads_dir."
