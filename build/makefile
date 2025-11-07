.PHONY: all clean

### Target Platform ####################
TARGET := zxn

### Project Name #######################
APPNAME := sysinfo

### Binary type ########################
APPTYPE := dotn

### OS specific settings ###############
ifeq ($(OS),Windows_NT)
RM := rm -f
MV := mv -f
else
RM := rm -f
MV := mv -f
endif

### Build Type #########################
BUILD ?= release

### Directories ########################
SRC_DIR = ../src
INC_DIR = ../inc
BUILD_DIR = .

### Source Files #######################
SRCS := $(wildcard $(SRC_DIR)/*.c)

### Compiler Options ###################
CFLAGS = -compiler=sdcc --vc -clib=sdcc_iy -SO3 --opt-code-size -I$(INC_DIR) -pragma-include:$(INC_DIR)/zpragma.inc

# Selecting CRT
CFLAGS += -startup=30

# verbose output
# CFLAGS += -v

ifeq ($(BUILD), debug)
# create list files
CFLAGS += --list

# add C code to list files
CFLAGS += --c-code-in-asm

# create debug code
CFLAGS += -D__DEBUG__
else ifeq ($(BUILD), release)
# speed vs. code quality
# CFLAGS += --max-allocs-per-node10000
endif

### Linker Options #####################
LDFLAGS = -subtype=dotn -Cz"--clean" -o$(BUILD_DIR)/$(APPNAME) -create-app

ifeq ($(BUILD), debug)
# create map file
LDFLAGS += -m

# create symbol files
LDFLAGS += -s
endif

### Compiler Command ###################
CC = zcc +$(TARGET) $(CFLAGS) $(SRCS) $(LDFLAGS)

### Create build target ################
all:
	$(CC)
  # Why fucking hell does ZCC not use the full name ?? 
ifeq ($(APPTYPE), dotn)
ifeq ($(OS),Windows_NT)
	@$(RM) $(BUILD_DIR)/$(APPNAME)
	@$(MV) $(BUILD_DIR)/SYSINF $(BUILD_DIR)/$(APPNAME)
endif
endif

### Cleanup build files ################
clean:
	@$(RM) $(BUILD_DIR)/$(APPNAME)
	@$(RM) $(wildcard $(BUILD_DIR)/*.lis)
	@$(RM) $(wildcard $(BUILD_DIR)/*.map)
	@$(RM) $(wildcard $(BUILD_DIR)/*.sym)
	@$(RM) $(wildcard $(SRC_DIR)/*.lis)
	@$(RM) $(wildcard $(SRC_DIR)/*.sym)
