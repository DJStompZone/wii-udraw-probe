ifeq ($(strip $(DEVKITPPC)),)
$(error "Please set DEVKITPPC in your environment. export DEVKITPRO=/opt/devkitpro and DEVKITPPC=$(DEVKITPRO)/devkitPPC")
endif

include $(DEVKITPPC)/wii_rules

TARGET      := udraw_probe
BUILD       := build
SOURCES     := source
INCLUDES    := include
DATA        := assets
TITLE       := uDraw Probe
AUTHOR      := OpenClaw
DESCRIPTION := Wii uDraw GameTablet probe app

CFILES      := $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
OFILES      := $(addprefix $(BUILD)/,$(CFILES:.c=.o))
HFILES      := $(foreach dir,$(INCLUDES),$(wildcard $(dir)/*.h))
LIBS        := -logc -lm

export INCLUDE := $(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) $(foreach dir,$(LIBOGC_INC),-I$(dir))
export LIBPATHS := $(foreach dir,$(LIBOGC_LIB),-L$(dir))

all: $(BUILD) $(TARGET).dol

$(BUILD):
	@mkdir -p $(BUILD)

$(BUILD)/%.o: $(SOURCES)/%.c $(HFILES)
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

$(TARGET).elf: $(OFILES)
	$(CC) $(LDFLAGS) $(OFILES) $(LIBPATHS) $(LIBS) -o $@

$(TARGET).dol: $(TARGET).elf
	$(ELF2DOL) $< $@

clean:
	rm -rf $(BUILD) $(TARGET).elf $(TARGET).dol

run: $(TARGET).dol
	wiiload $(TARGET).dol

.PHONY: all clean run
