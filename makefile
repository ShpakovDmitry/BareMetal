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
OBJSIZE := $(ARCH)-size

SOURCE_DIR := source
BUILD_DIR := build
OBJECT_DIR := $(BUILD_DIR)/object
LINKER_SCRIPT_DIR := etc/linker
DIRS := $(BUILD_DIR) $(OBJECT_DIR)

LINKER_SCRIPT_FILE := $(LINKER_SCRIPT_DIR)/linkerFile.ld
CC_SRC_FILES := $(wildcard $(SOURCE_DIR)/*.c)
AS_SRC_FILES := $(wildcard $(SOURCE_DIR)/*.s)
CC_OBJ_FILES := $(CC_SRC_FILES:$(SOURCE_DIR)/%.c=$(OBJECT_DIR)/%.o)
AS_OBJ_FILES := $(AS_SRC_FILES:$(SOURCE_DIR)/%.s=$(OBJECT_DIR)/%.o)

CC_FLAGS := -c -Wall -Werror -idirafter ./include -mmcu=$(MCU)
CC_FLAGS += -nostartfiles -nodefaultlibs -nostdlib -Os
CC_FLAGS += -fdata-sections -ffunction-sections
CC_FLAGS += -ffreestanding
CC_FLAGS += -fms-extensions -fno-exceptions
CC_FLAGS += -fstrict-volatile-bitfields -Wextra
CC_FLAGS += -Wcast-align -Wconversion -Wsign-conversion
CC_FLAGS += -Wshadow -Wlogical-op -Wsuggest-final-types
CC_FLAGS += -Wsuggest-final-methods -pedantic
AS_FLAGS := -mmcu=$(MCU_ARCH) -I ./include
LD_FLAGS := -T $(LINKER_SCRIPT_FILE) -Map $(BUILD_DIR)/$(TARGET).map
OBJDUMP_FLAGS := --disassemble-all -m $(MCU_ARCH) --private=mem-usage $(BUILD_DIR)/$(TARGET).elf > $(BUILD_DIR)/$(TARGET).s
OBJCOPY_FLAGS := -O ihex $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex
OBJSIZE_FLAGS := 



$(OBJECT_DIR)/%.o: $(SOURCE_DIR)/%.c
	$(CC) $(CC_FLAGS) $^ -o $@

$(OBJECT_DIR)/%.o: $(SOURCE_DIR)/%.s
	$(AS) $(AS_FLAGS) $^ -o $@

$(BUILD_DIR)/firmware.elf: $(AS_OBJ_FILES) $(CC_OBJ_FILES)
	$(LD) $(LD_FLAGS) $^ -o $@
	$(OBJDUMP) $(OBJDUMP_FLAGS)
	$(OBJCOPY) $(OBJCOPY_FLAGS)
	$(OBJSIZE) $(OBJSIZE_FLAGS) $(BUILD_DIR)/$(TARGET).elf

$(BUILD_DIR):
	mkdir --parents $(DIRS)

clean:
	rm -rf $(DIRS)

all: $(BUILD_DIR) $(BUILD_DIR)/$(TARGET).elf
