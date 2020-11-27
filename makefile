.PHONY := all clean flash
.DEFAULT_GOAL := all

TARGET := firmware
ARCH := arm-none-eabi
MCU := cortex-m4

CC := $(ARCH)-gcc
CXX := $(ARCH)-g++
AS := $(ARCH)-as
LD := $(ARCH)-ld
OBJDUMP := $(ARCH)-objdump
OBJCOPY := $(ARCH)-objcopy
OBJSIZE := $(ARCH)-size

SOURCE_DIR := source
BUILD_DIR := build
OBJECT_DIR := $(BUILD_DIR)/object
LINKER_SCRIPT_DIR := etc/linker
DIRS := $(BUILD_DIR) $(OBJECT_DIR)

LINKER_SCRIPT_FILE := $(LINKER_SCRIPT_DIR)/nrf52840.ld
CXX_SRC_FILES := $(wildcard $(SOURCE_DIR)/*.cpp)
CXX_SRC_FILES += source/runtime/isr.cpp
CXX_SRC_FILES += source/runtime/startup.cpp
CXX_OBJ_FILES := $(CXX_SRC_FILES:$(SOURCE_DIR)/%.cpp=$(OBJECT_DIR)/%.o)

CXX_FLAGS := -c -Wall -Werror -mcpu=$(MCU) -mthumb -Os -std=c++17
CXX_FLAGS += -idirafter ./include -fdata-sections -ffunction-sections
CXX_FLAGS += -ffreestanding -v -nostdinc -idirafter ./include/stdlib
CXX_FLAGS += -fms-extensions -fno-exceptions -fno-rtti -fno-use-cxa-atexit
CXX_FLAGS += -fno-threadsafe-statics -fstrict-volatile-bitfields -Wextra
CXX_FLAGS += -Wcast-align -Wconversion -Wsign-conversion -Wold-style-cast
CXX_FLAGS += -Wshadow -Wlogical-op -Wsuggest-override -Wsuggest-final-types
CXX_FLAGS += -Wsuggest-final-methods -pedantic
LD_FLAGS := -T $(LINKER_SCRIPT_FILE) -Map $(BUILD_DIR)/$(TARGET).map --gc-sections
LD_FLAGS += -nostartfiles -nolibc -nostdlib -nodefaultfiles
OBJDUMP_FLAGS := --disassemble-all
OBJCOPY_FLAGS := -O ihex 
OBJSIZE_FLAGS := 

$(OBJECT_DIR)/%.o: $(SOURCE_DIR)/%.cpp
	$(CXX) $(CXX_FLAGS) $^ -o $@
	$(OBJSIZE) $(OBJSIZE_FLAGS) $@ > $(@:%.o=%.s)
	$(OBJDUMP) $(OBJDUMP_FLAGS) $@ >> $(@:%.o=%.s)

$(BUILD_DIR)/$(TARGET).elf: $(AS_OBJ_FILES) $(CXX_OBJ_FILES)
	$(LD) $(LD_FLAGS) $^ -o $@
	$(OBJSIZE) $(OBJSIZE_FLAGS) $@ > $(BUILD_DIR)/$(TARGET).s
	$(OBJDUMP) $(OBJDUMP_FLAGS) $@ >> $(BUILD_DIR)/$(TARGET).s
	$(OBJCOPY) $(OBJCOPY_FLAGS) $@ $(BUILD_DIR)/$(TARGET).hex
	$(OBJSIZE) $(OBJSIZE_FLAGS) $@

$(BUILD_DIR):
	mkdir --parents $(DIRS)

clean:
	rm -rf $(DIRS)

all: $(BUILD_DIR) $(BUILD_DIR)/$(TARGET).elf

flash: all
	nrfjprog -f nrf52 --program build/firmware.hex --sectorerase
	nrfjprog -f nrf52 --pinreset
