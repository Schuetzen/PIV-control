# Makefile for Basler pylon sample program
.PHONY: all clean

# The program to build
NAME       := piv

LIBS = -lwiringPi -pthread


# Installation directories for pylon
PYLON_ROOT ?= /opt/pylon

# Build tools and flags
LD         := $(CXX)
CPPFLAGS   := $(shell $(PYLON_ROOT)/bin/pylon-config --cflags)
CXXFLAGS   := #e.g., CXXFLAGS=-g -O0 for debugging
LDFLAGS    := $(shell $(PYLON_ROOT)/bin/pylon-config --libs-rpath)
LDLIBS     := $(shell $(PYLON_ROOT)/bin/pylon-config --libs)

# Directories
BUILD_DIR := ../build

# Rules for building
all: $(BUILD_DIR)/$(NAME)

$(BUILD_DIR)/$(NAME): $(BUILD_DIR)/$(NAME).o
	$(LD) $(LDFLAGS) -o $@ $^ $(LDLIBS) $(LIBS) $(LIBS)

$(BUILD_DIR)/$(NAME).o: $(NAME).cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<

clean:
	$(RM) $(BUILD_DIR)/$(NAME).o $(BUILD_DIR)/$(NAME)
