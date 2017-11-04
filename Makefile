# Basic user settings

TARGET     := timers

SRC_DIR    := src
INC_DIR    := inc
BLD_DIR    := build

CFLAGS     := -I$(INC_DIR) --stack-auto --disable-warning 154
LFLAGS     := --code-loc 0x2100 --xram-loc 0x6000 --stack-auto --stack-loc 0x80 

M3P_LOADER := toolchain/load.m3p
BAUD	   := 9600


# OS dependent settings

IS_WINDOWS := $(if $(wildcard C:\Windows),YES,NO)

ifeq ($(IS_WINDOWS),YES)
BLD_DIR    := $(BLD_DIR).windows
CC         := "C:\Program Files\SDCC\bin\sdcc.exe"
SED        := $(abspath .\toolchain\windows\bin\sed.exe)
M3P		   := $(abspath .\toolchain\windows\bin\M3P.exe)
RM         := rmdir /S /Q
PORT	   := com1
SILENT     := > nul
else
BLD_DIR    := $(BLD_DIR).linux
CC         := sdcc
SED        := sed
M3P		   := gm3p
PORT	   := usb0
SILENT     := >/dev/null
RM         := rm -rf
endif


# Internal setup

TARGET_BIN := $(BLD_DIR)/$(TARGET).bin
TARGET_HEX := $(BLD_DIR)/$(TARGET).hex

M3P_LOADER_COOKED := $(BLD_DIR)/$(notdir $(M3P_LOADER))

SOURCES := $(wildcard $(SRC_DIR)/*.c)
HEADERS := $(wildcard $(INC_DIR)/*.h)
OBJECTS := $(patsubst $(SRC_DIR)/%.c,$(BLD_DIR)/%.rel,$(SOURCES))


# Rules

.PHONY: all
all: build

.PHONY: build
build: $(TARGET_BIN)

$(TARGET_BIN): $(TARGET_HEX) $(MAKEFILE_LIST) | $(BLD_DIR)
	cd $(BLD_DIR) && $(M3P) hb166 $(abspath $<) $(abspath $@) bye $(SILENT)

$(TARGET_HEX): $(OBJECTS) $(MAKEFILE_LIST) | $(BLD_DIR)
	$(CC) $(LFLAGS) $(OBJECTS) -o $@

$(BLD_DIR)/%.rel: $(SRC_DIR)/%.c $(HEADERS) $(MAKEFILE_LIST) | $(BLD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: load
load: $(M3P_LOADER_COOKED) build
	cd $(BLD_DIR) && $(M3P) lfile $(abspath $<)

$(M3P_LOADER_COOKED): $(M3P_LOADER) $(MAKEFILE_LIST) | $(BLD_DIR)
	$(SED) \
		-e s!@@BAUD@@!$(BAUD)!g \
		-e s!@@PORT@@!$(PORT)!g \
		-e s!@@BINARY@@!$(notdir $(TARGET_BIN))!g \
		$< > $@

.PHONY: clean
clean: $(BLD_DIR)
	$(RM) $(BLD_DIR)

$(BLD_DIR):
	mkdir $@