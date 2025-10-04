.PHONY: clean all

# Zielplattform
TARGET = zxn

# Name der Ausgabe-Datei
APPNAME = sysinfo

# Verzeichnisse
SRC_DIR = ../src
INC_DIR = ../inc
BUILD_DIR = .

# Quellcode-Dateien
SRCS  = $(SRC_DIR)/main.c
SRCS += $(SRC_DIR)/dumpDos.c
SRCS += $(SRC_DIR)/dumpRegs.c
SRCS += $(SRC_DIR)/dumpVars.c

# Compiler-Flags
CFLAGS  = -compiler=sdcc -clib=sdcc_iy -SO3 --opt-code-size -I$(INC_DIR) -pragma-include:$(INC_DIR)/zpragma.inc
CFLAGS += -startup=30
# CFLAGS += --max-allocs-per-node200000
# CFLAGS += --max-allocs-per-node4000
CFLAGS += -v           # verbose output
# CFLAGS += --list       # create list files
# CFLAGS += --fverbose-asm
# CFLAGS += -D__DEBUG__  # create debug code

# Linker-Flags
LDFLAGS  = -subtype=dotn -Cz"--clean" -o$(BUILD_DIR)/$(APPNAME) -create-app  
# LDFLAGS += -m  # create map file
# LDFLAGS += -s  # create symbol files

# Compiler-Befehl
CC = zcc +$(TARGET) $(CFLAGS) $(SRCS) $(LDFLAGS)

# Create build target
all:
	$(CC)
  # Why fucking hell does ZCC not use the full name ?? 
	@rm -f $(BUILD_DIR)/$(APPNAME)
	@mv -f $(BUILD_DIR)/SYSINF $(BUILD_DIR)/$(APPNAME)

# Cleanup all build files
clean:
	@rm -f $(BUILD_DIR)/$(APPNAME)
	@rm -f $(BUILD_DIR)/$(APPNAME).lis
	@rm -f $(BUILD_DIR)/$(APPNAME).map
	@rm -f $(BUILD_DIR)/$(APPNAME).sym
	@rm -f $(SRC_DIR)/*.lis
	@rm -f $(SRC_DIR)/*.sym
