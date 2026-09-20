.DEFAULT_GOAL := all
.DELETE_ON_ERROR:

TARGET ?= stm32-lwip-eth
CONFIG ?= Debug

CROSS_COMPILE ?= arm-none-eabi-
CC      := $(CROSS_COMPILE)gcc
SIZE    := $(CROSS_COMPILE)size
OBJDUMP := $(CROSS_COMPILE)objdump
OBJCOPY := $(CROSS_COMPILE)objcopy

LINKER_SCRIPT      := STM32F767ZITX_FLASH.ld
override BUILD_ROOT := build
override BUILD_DIR  := $(BUILD_ROOT)/$(CONFIG)

ELF  := $(BUILD_DIR)/$(TARGET).elf
MAP  := $(BUILD_DIR)/$(TARGET).map
BIN  := $(BUILD_DIR)/$(TARGET).bin
LIST := $(BUILD_DIR)/$(TARGET).list

# New C files placed directly in these directories are picked up automatically.
# Add a directory here when introducing a new source module elsewhere.
SOURCE_DIRS_AFTER_STARTUP := \
	Drivers/STM32F7xx_HAL_Driver/Src \
	LWIP/App \
	LWIP/Target \
	Middlewares/Third_Party/LwIP/src/api \
	Middlewares/Third_Party/LwIP/src/apps/mqtt \
	Middlewares/Third_Party/LwIP/src/core \
	Middlewares/Third_Party/LwIP/src/core/ipv4 \
	Middlewares/Third_Party/LwIP/src/core/ipv6 \
	Middlewares/Third_Party/LwIP/src/netif \
	Middlewares/Third_Party/LwIP/src/netif/ppp

CORE_SOURCES := $(wildcard Core/Src/*.c)
ASM_SOURCES  := Core/Startup/startup_stm32f767zitx.s
OTHER_SOURCES := $(foreach dir,$(SOURCE_DIRS_AFTER_STARTUP),$(wildcard $(dir)/*.c))

# Space-separated paths may be supplied on the make command line if a source in
# one of the directories above must be omitted in a particular build.
EXCLUDED_SOURCES ?=
SOURCES := $(filter-out $(EXCLUDED_SOURCES),$(CORE_SOURCES) $(ASM_SOURCES) $(OTHER_SOURCES))

OBJECTS := $(addprefix $(BUILD_DIR)/,$(SOURCES))
OBJECTS := $(OBJECTS:.c=.o)
OBJECTS := $(OBJECTS:.s=.o)
DEPENDENCIES := $(OBJECTS:.o=.d)

DEFINES := \
	USE_HAL_DRIVER \
	STM32F767xx

INCLUDE_DIRS := \
	Middlewares/Third_Party/LwIP/src/apps \
	Middlewares/Third_Party/LwIP/src/include/lwip/apps \
	Middlewares/Third_Party/LwIP/src/include/lwip \
	Middlewares/Third_Party/LwIP/system/arch \
	Middlewares/Third_Party/LwIP/src/include/netif \
	Middlewares/Third_Party/LwIP/src/include/lwip/prot \
	Middlewares/Third_Party/LwIP/src/include/lwip/priv \
	Middlewares/Third_Party/LwIP/src/apps/http \
	Core/Inc \
	Drivers/CMSIS/Include \
	Drivers/CMSIS/Device/ST/STM32F7xx/Include \
	Drivers/STM32F7xx_HAL_Driver/Inc \
	Drivers/STM32F7xx_HAL_Driver/Inc/Legacy \
	LWIP/App \
	LWIP/Target \
	Middlewares/Third_Party/LwIP/src/include \
	Middlewares/Third_Party/LwIP/system

CPPFLAGS := \
	$(addprefix -D,$(DEFINES)) \
	$(addprefix -I,$(INCLUDE_DIRS))

MCU_FLAGS := \
	-mcpu=cortex-m7 \
	-mfpu=fpv5-d16 \
	-mfloat-abi=hard \
	-mthumb

COMMON_CFLAGS := \
	$(MCU_FLAGS) \
	-std=gnu11 \
	-ffunction-sections \
	-fdata-sections \
	-Wall \
	-fstack-usage \
	-MMD \
	-MP \
	--specs=nano.specs

ifeq ($(CONFIG),Debug)
CONFIG_CFLAGS := -Og -g3
CONFIG_ASFLAGS := -g3
else ifeq ($(CONFIG),Release)
CONFIG_CFLAGS := -Os -g0
CONFIG_ASFLAGS := -g0
else
$(error Unsupported CONFIG "$(CONFIG)"; use Debug or Release)
endif

CFLAGS := $(COMMON_CFLAGS) $(CONFIG_CFLAGS)

ASFLAGS := \
	$(MCU_FLAGS) \
	$(CONFIG_ASFLAGS) \
	-x assembler-with-cpp \
	-MMD \
	-MP \
	--specs=nano.specs

LDFLAGS := \
	$(MCU_FLAGS) \
	-T$(LINKER_SCRIPT) \
	--specs=nosys.specs \
	-Wl,-Map=$(MAP) \
	-Wl,--gc-sections \
	-static \
	--specs=nano.specs

LDLIBS := \
	-Wl,--start-group \
	-lc \
	-lm \
	-Wl,--end-group

.PHONY: all size clean clean-all

all: $(ELF) $(BIN) $(LIST) size

$(ELF): $(OBJECTS) $(LINKER_SCRIPT)
	@mkdir -p $(dir $@)
	$(CC) -o $@ $(OBJECTS) $(LDFLAGS) $(LDLIBS)

$(BIN): $(ELF)
	$(OBJCOPY) -O binary $< $@

$(LIST): $(ELF)
	$(OBJDUMP) -h -S $< > $@

size: $(ELF)
	$(SIZE) $<

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $< $(CFLAGS) $(CPPFLAGS) \
		-MF $(@:.o=.d) -MT $@ -c -o $@

$(BUILD_DIR)/%.o: %.s
	@mkdir -p $(dir $@)
	$(CC) $(ASFLAGS) \
		-MF $(@:.o=.d) -MT $@ -c $< -o $@

clean:
	rm -rf build/$(CONFIG)

clean-all:
	rm -rf build/Debug build/Release

-include $(DEPENDENCIES)
