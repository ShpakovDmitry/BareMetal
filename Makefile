.PHONY := all clean
.DEFAULT_GOAL := all

TARGET := firmware
ARCH := arm-none-eabi
MCU := cortex-m4

CC := $(ARCH)-gcc
AS := $(ARCH)-as
LD := $(ARCH)-ld
OBJDUMP := $(ARCH)-objdump
OBJCOPY := $(ARCH)-objcopy
OBJSIZE := $(ARCH)-size

SOURCE_DIR := ./source
BUILD_DIR := ./build
OBJECT_DIR := $(BUILD_DIR)
DIRS := $(BUILD_DIR)
LD_SCRIPT_DIR := etc/linker

CC_SRC_FILES := $(wildcard $(SOURCE_DIR)/*.c)
CC_OBJ_FILES := $(CC_SRC_FILES:$(SOURCE_DIR)/%.c=$(OBJECT_DIR)/%.o)

CC_FLAGS := -c -Wall -Werror -mcpu=$(MCU) -mthumb -Os -std=c18
CC_FLAGS += -fdata-sections -ffunction-sections -ffreestanding -nostdinc
CC_FLAGS += -idirafter ./include
LD_SCRIPT_FILE := $(LD_SCRIPT_DIR)/nrf52840.ld
LD_FLAGS := -T $(LD_SCRIPT_FILE) -Map $(BUILD_DIR)/$(TARGET).map --gc-sections
LD_FLAGS += -nostartfiles -nolibc -nostdlib -nodefaultfiles

OBJDUMP_FLAGS := --disassemble-all -z
OBJCOPY_FLAGS := -O ihex 
OBJSIZE_FLAGS := 

$(OBJECT_DIR)/%.o: $(SOURCE_DIR)/%.c
	$(CC) $(CC_FLAGS) $^ -o $@
	$(OBJSIZE) $(OBJSIZE_FLAGS) $@ > $(@:%.o=%.s)
	$(OBJDUMP) $(OBJDUMP_FLAGS) $@ >> $(@:%.o=%.s)

target: $(CC_OBJ_FILES)
	$(LD) $(LD_FLAGS) $(CC_OBJ_FILES) -o $(BUILD_DIR)/$(TARGET).elf
	$(OBJSIZE) $(OBJSIZE_FLAGS) $(BUILD_DIR)/$(TARGET).elf > $(BUILD_DIR)/$(TARGET).s
	$(OBJDUMP) $(OBJDUMP_FLAGS) $(BUILD_DIR)/$(TARGET).elf >> $(BUILD_DIR)/$(TARGET).s
	$(OBJCOPY) $(OBJCOPY_FLAGS) $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex
	$(OBJSIZE) $(OBJSIZE_FLAGS) $(BUILD_DIR)/$(TARGET).elf


$(BUILD_DIR):
	mkdir --parents $(DIRS)

clean:
	rm -rf $(DIRS)

all: $(BUILD_DIR) $(CC_OBJ_FILES) target

flash: all
	nrfjprog -f nrf52 --program build/firmware.hex --sectorerase
	nrfjprog -f nrf52 --pinreset
