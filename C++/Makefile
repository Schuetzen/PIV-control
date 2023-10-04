# Makefile for Basler pylon C++ sample programs
.PHONY            : all clean

# The programs to build
SUBDIRS           := DeviceRemovalHandling \
                     Grab \
                     Grab_CameraEvents \
                     Grab_ChunkImage \
                     Grab_MultiCast \
                     Grab_MultipleCameras \
                     Grab_Strategies \
                     Grab_UsingActionCommand \
                     Grab_UsingBufferFactory \
                     Grab_UsingExposureEndEvent \
                     Grab_UsingGrabLoopThread \
                     Grab_UsingSequencer \
                     ParametrizeCamera_AutoFunctions \
                     ParametrizeCamera_Configurations \
                     ParametrizeCamera_GenericParameterAccess \
                     ParametrizeCamera_LoadAndSave \
                     ParametrizeCamera_LookupTable \
                     ParametrizeCamera_NativeParameterAccess \
                     ParametrizeCamera_SerialCommunication \
                     ParametrizeCamera_Shading \
                     ParametrizeCamera_UserSets \
                     Utility_GrabVideo \
                     Utility_Image \
                     Utility_ImageDecompressor \
                     Utility_ImageFormatConverter \
                     Utility_ImageLoadAndSave \
                     Utility_IpConfig \
                     Utility_InstantInterface

# check if PYLON_ROOT is set
ifndef PYLON_ROOT
$(error PYLON_ROOT is not set)
endif

# Rules for building
all:
	for subDirectory in $(SUBDIRS); do \
		$(MAKE) -C $$subDirectory PYLON_ROOT="$(PYLON_ROOT)" all; \
		if test $$? -ne 0; then exit 1; fi; \
	done

clean:
	for subDirectory in $(SUBDIRS); do \
		$(MAKE) -C $$subDirectory PYLON_ROOT="$(PYLON_ROOT)" clean; \
	done

.PHONY: install
install:
