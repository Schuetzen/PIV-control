# Makefile for Basler pylon sample program
.PHONY: all clean

# The program to build
NAME       := piv

# Object files
OBJS       := $(NAME).o UtilityFunctions.o 

# Libraries
LIBS       := -lwiringPi -pthread

# Installation directories for pylon
PYLON_ROOT ?= /opt/pylon

# Build tools and flags
LD         := $(CXX)
CPPFLAGS   := $(shell $(PYLON_ROOT)/bin/pylon-config --cflags)
CXXFLAGS   := # e.g., CXXFLAGS=-g -O0 for debugging
LDFLAGS    := $(shell $(PYLON_ROOT)/bin/pylon-config --libs-rpath)
LDLIBS     := $(shell $(PYLON_ROOT)/bin/pylon-config --libs)

# Rules for building
all: $(NAME)

$(NAME): $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $^ $(LDLIBS) $(LIBS)

$(NAME).o: $(NAME).cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<

UtilityFunctions.o: UtilityFunctions.cpp UtilityFunctions.h
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ UtilityFunctions.cpp

SignalFunction.o: SignalFunction.cpp SignalFunction.h
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ SignalFunction.cpp

pivGrabFunction.o: pivGrabFunction.cpp pivGrabFunction.h
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ pivGrabFunction.cpp

clean:
	$(RM) $(OBJS) $(NAME)
