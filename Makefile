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
LIBS        := -lfat -lwiiuse -lbte -logc -lm

export INCLUDE  := $(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) $(foreach dir,$(LIBOGC_INC),-I$(dir))
export LIBPATHS := $(foreach dir,$(LIBOGC_LIB),-L$(dir))
export MACHDEP  := -DGEKKO -mrvl -mcpu=750 -meabi -mhard-float
export CFLAGS   := -g -O2 -Wall -Wextra -std=c11 $(MACHDEP)
export LDFLAGS  := -g $(MACHDEP)
export ELF2DOL  := $(DEVKITPRO)/tools/bin/elf2dol

all: $(BUILD) $(TARGET).dol

$(BUILD):
	@mkdir -p $(BUILD)

$(BUILD)/%.o: $(SOURCES)/%.c $(HFILES)
	$(CC) -MMD -MP $(CFLAGS) $(INCLUDE) -c $< -o $@

$(TARGET).elf: $(OFILES)
	$(CC) $(OFILES) $(LDFLAGS) $(LIBPATHS) $(LIBS) -o $@

$(TARGET).dol: $(TARGET).elf
	$(ELF2DOL) $< $@

clean:
	rm -rf $(BUILD) $(TARGET).elf $(TARGET).dol

run: $(TARGET).dol
	wiiload $(TARGET).dol

-include $(OFILES:.o=.d)

.PHONY: all clean run
