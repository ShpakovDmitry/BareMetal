.PHONY := all clean
.DEFAULT_GOAL := all

ARCH := arm-none-eabi
MCU := cortex-m4

CC := $(ARCH)-gcc
CXX := $(ARCH)-g++
AS := $(ARCH)-as
LD := $(ARCH)-ld
OBJDUMP := $(ARCH)-objdump
OBJCOPY := $(ARCH)-objcopy
OBJSIZE := $(ARCH)-size

SOURCE_DIR := ./
BUILD_DIR := build
OBJECT_DIR := $(BUILD_DIR)
DIRS := $(BUILD_DIR)

CXX_SRC_FILES := $(wildcard $(SOURCE_DIR)/*.cpp)
CXX_OBJ_FILES := $(CXX_SRC_FILES:$(SOURCE_DIR)/%.cpp=$(OBJECT_DIR)/%.o)

CXX_FLAGS := -c -Wall -Werror -mcpu=$(MCU) -mthumb -Os -std=c++17
CXX_FLAGS += -idirafter ../../source -fdata-sections -ffunction-sections
CXX_FLAGS += -ffreestanding -idirafter ../../include
CXX_FLAGS += -fms-extensions -fno-exceptions -fno-rtti -fno-use-cxa-atexit
CXX_FLAGS += -fno-threadsafe-statics -fstrict-volatile-bitfields -Wextra
CXX_FLAGS += -Wcast-align -Wconversion -Wsign-conversion -Wold-style-cast
CXX_FLAGS += -Wshadow -Wlogical-op -Wsuggest-override -Wsuggest-final-types
CXX_FLAGS += -Wsuggest-final-methods -pedantic
OBJDUMP_FLAGS := --disassemble-all -z
OBJCOPY_FLAGS := -O ihex 
OBJSIZE_FLAGS := 

$(OBJECT_DIR)/%.o: $(SOURCE_DIR)/%.cpp
	$(CXX) $(CXX_FLAGS) $^ -o $@
	$(OBJSIZE) $(OBJSIZE_FLAGS) $@ > $(@:%.o=%.s)
	$(OBJDUMP) $(OBJDUMP_FLAGS) $@ >> $(@:%.o=%.s)

$(BUILD_DIR):
	mkdir --parents $(DIRS)

clean:
	rm -rf $(DIRS)

all: $(BUILD_DIR) $(CXX_OBJ_FILES)
