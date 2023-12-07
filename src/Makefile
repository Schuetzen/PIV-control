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
OBJ_DIR    := ../build
BIN_DIR    := ../bin

# Object Files
OBJ_FILES  := $(OBJ_DIR)/$(NAME).o

# Rules for building
all: $(BIN_DIR)/$(NAME)

$(BIN_DIR)/$(NAME): $(OBJ_FILES)
	$(LD) $(LDFLAGS) -o $@ $^ $(LDLIBS) $(LIBS) $(LIBS)

$(OBJ_DIR)/$(NAME).o: $(NAME).cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<

clean:
	$(RM) $(OBJ_FILES) $(BIN_DIR)/$(NAME)
