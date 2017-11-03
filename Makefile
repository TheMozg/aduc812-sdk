CC      = "C:\Program Files\SDCC\bin\sdcc.exe"
CFLAGS  = -I$(INC_DIR) --stack-auto
LFLAGS  = --code-loc 0x2100 --xram-loc 0x6000 --stack-auto --stack-loc 0x80 


M3P		 = .\toolchain\windows\M3P.exe
COMPORT	 = com1
BAUD	 = 9600	


SRC_DIR    := src
BLD_DIR    := bld
INC_DIR    := inc
TARGET     := test_led
TARGET_BIN := $(BLD_DIR)/$(TARGET).bin
TARGET_HEX := $(BLD_DIR)/$(TARGET).hex

M3P_LOAD_SCRIPT := load.m3p
M3P_LOAD_SCRIPT_COOKED := $(BLD_DIR)/$(M3P_LOAD_SCRIPT)

SOURCES := $(wildcard $(SRC_DIR)/*.c)
HEADERS := $(wildcard $(INC_DIR)/*.h)
OBJECTS := $(patsubst $(SRC_DIR)/%,$(BLD_DIR)/%,$(SOURCES))
OBJECTS := $(patsubst %.c,%.rel,$(OBJECTS))


.PHONY: all
all: build

.PHONY: build
build: $(TARGET_BIN)

$(TARGET_BIN): $(TARGET_HEX) $(MAKEFILE_LIST)
	$(M3P) hb166 $< $@ bye

$(TARGET_HEX): $(OBJECTS) $(MAKEFILE_LIST)
	$(CC) $(LFLAGS) $(OBJECTS) -o $@

$(BLD_DIR)/%.rel: $(SRC_DIR)/%.c $(HEADERS) $(MAKEFILE_LIST) | $(BLD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BLD_DIR):
	mkdir $@

.PHONY: load
load: $(M3P_LOAD_SCRIPT_COOKED)
	$(M3P) lfile $(M3P_LOAD_SCRIPT_COOKED)

$(M3P_LOAD_SCRIPT_COOKED): $(M3P_LOAD_SCRIPT)
	sed $< 's/@PORT@/$(PORT)/g' > $@