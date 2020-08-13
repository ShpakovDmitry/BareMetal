.PHONY := all clean
.DEFAULT_GOAL := all

TARGET := firmware

ARCH := avr
MCU := attiny2313
MCU_ARCH := $(ARCH)25

CC := $(ARCH)-gcc
AS := $(ARCH)-as
LD := $(ARCH)-ld
OBJDUMP := $(ARCH)-objdump
OBJCOPY := $(ARCH)-objcopy

SOURCE_DIR := source
RUNTIME_DIR := $(SOURCE_DIR)/runtime
BUILD_DIR := build
OBJECT_DIR := $(BUILD_DIR)/object
LINKER_SCRIPT_DIR := etc/linker
DIRS := $(BUILD_DIR) $(OBJECT_DIR) $(OBJECT_DIR)/runtime $(OBJECT_DIR)/cc

LINKER_SCRIPT_FILE := $(LINKER_SCRIPT_DIR)/linkerFile.ld
CC_SRC_FILES := $(wildcard $(SOURCE_DIR)/*.c)
AS_SRC_FILES := $(wildcard $(RUNTIME_DIR)/*.s)
CC_OBJ_FILES := $(CC_SRC_FILES:$(SOURCE_DIR)/%.c=$(OBJECT_DIR)/cc/%.o)
AS_OBJ_FILES := $(AS_SRC_FILES:$(RUNTIME_DIR)/%.s=$(OBJECT_DIR)/runtime/%.o)

CC_FLAGS := -c -Wall -Werror -idirafter ./include -mmcu=$(MCU) -nostartfiles -nostdlib -lgcc
AS_FLAGS := -mmcu=$(MCU_ARCH) -I ../../include
LD_FLAGS := -T $(LINKER_SCRIPT_FILE) -Map $(BUILD_DIR)/$(TARGET).map
OBJDUMP_FLAGS := --disassemble-all -m $(MCU_ARCH) --private=mem-usage $(BUILD_DIR)/$(TARGET).elf > $(BUILD_DIR)/$(TARGET).s
OBJCOPY_FLAGS := -O ihex $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex


$(OBJECT_DIR)/cc/%.o: $(SOURCE_DIR)/%.c
	$(CC) $(CC_FLAGS) $^ -o $@

$(OBJECT_DIR)/runtime/%.o: $(RUNTIME_DIR)/%.s
	cd $(RUNTIME_DIR) && $(AS) $(AS_FLAGS) ../../$^ -o ../../$@

$(BUILD_DIR)/firmware.elf: $(AS_OBJ_FILES) $(CC_OBJ_FILES)
	$(LD) $(LD_FLAGS) $^ -o $@
	$(OBJDUMP) $(OBJDUMP_FLAGS)
	$(OBJCOPY) $(OBJCOPY_FLAGS)
$(BUILD_DIR):
	mkdir --parents $(DIRS)

clean:
	rm -rf $(DIRS)

all: $(BUILD_DIR) $(BUILD_DIR)/$(TARGET).elf
