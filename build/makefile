.PHONY: clean all

# Target Platform ä###########
TARGET = zxn

# Project Name ###############
APPNAME = sysinfo

# Directories ################
SRC_DIR = ../src
INC_DIR = ../inc
BUILD_DIR = .

# Source Files ###############
SRCS  = $(SRC_DIR)/main.c
SRCS += $(SRC_DIR)/dumpDos.c
SRCS += $(SRC_DIR)/dumpRegs.c
SRCS += $(SRC_DIR)/dumpVars.c

# Compiler Options ###########
CFLAGS = -compiler=sdcc --vc -clib=sdcc_iy -SO3 --opt-code-size -I$(INC_DIR) -pragma-include:$(INC_DIR)/zpragma.inc

# Selecting CRT
CFLAGS += -startup=30

# speed vs. code quality
# CFLAGS += --max-allocs-per-node200000

# verbose output
# CFLAGS += -v

# create list files
# CFLAGS += --list

# add C code to list files
# CFLAGS += --c-code-in-asm

# create debug code
# CFLAGS += -D__DEBUG__

# Linker Options #############
LDFLAGS = -subtype=dotn -Cz"--clean" -o$(BUILD_DIR)/$(APPNAME) -create-app

# create map file
# LDFLAGS += -m

# create symbol files
# LDFLAGS += -s

# Compiler Command ###########
CC = zcc +$(TARGET) $(CFLAGS) $(SRCS) $(LDFLAGS)

# Create build target ########
all:
	$(CC)
  # Why fucking hell does ZCC not use the full name ?? 
	@rm -f $(BUILD_DIR)/$(APPNAME)
	@mv -f $(BUILD_DIR)/SYSINF $(BUILD_DIR)/$(APPNAME)

# Cleanup build files ########
clean:
	@rm -f $(BUILD_DIR)/$(APPNAME)
	@rm -f $(BUILD_DIR)/$(APPNAME).lis
	@rm -f $(BUILD_DIR)/$(APPNAME).map
	@rm -f $(BUILD_DIR)/$(APPNAME).sym
	@rm -f $(SRC_DIR)/*.lis
	@rm -f $(SRC_DIR)/*.sym
