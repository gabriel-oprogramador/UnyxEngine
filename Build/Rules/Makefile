include Build/Rules/Common.mk

SELF_MK      = Build/Rules/Makefile
PLATFORM_MK  = Build/Rules/$(PLATFORM).mk

TARGET ?= Sandbox
TARGET_DIR := $(shell find Source -type d -name "$(TARGET)" -print -quit)
TARGET_MK  := $(TARGET_DIR)/Target.mk
ENGINE_MK  := Source/Engine/Target.mk
EDITOR_MK  := Source/Programs/Editor/Target.mk

ifeq ($(TARGET_DIR),)
$(error [Unyx] Target "$(TARGET)" could not be located under Source/)
endif

ifeq ($(wildcard $(TARGET_MK)),)
$(error [Unyx] Directory "$(TARGET_DIR)" exists, but Target.mk is missing)
endif

all: build

generate_cc_json:
	$(GENERATE_COMPILE_COMMANDS)

build:
	$(MAKE) -f $(SELF_MK) start TARGET_MK=$(TARGET_MK) PLATFORM=$(PLATFORM) BUILD_TYPE=$(BUILD_TYPE)

run:
	$(MAKE) -f Build/Rules/$(PLATFORM).mk run TARGET_MK=$(TARGET_MK) PLATFORM=$(PLATFORM) BUILD_TYPE=$(BUILD_TYPE)

dev:
	$(MAKE) -f $(SELF_MK) start TARGET_MK=$(ENGINE_MK) PLATFORM=$(PLATFORM) BUILD_TYPE=Development
	$(MAKE) -f $(SELF_MK) start TARGET_MK=$(TARGET_MK) PLATFORM=$(PLATFORM) BUILD_TYPE=Development

ship:
	$(MAKE) -f $(SELF_MK) start TARGET_MK=$(ENGINE_MK) PLATFORM=$(PLATFORM) BUILD_TYPE=Shipping
	$(MAKE) -f $(SELF_MK) start TARGET_MK=$(TARGET_MK) PLATFORM=$(PLATFORM) BUILD_TYPE=Shipping

start:
	$(MAKE) -f Build/Rules/$(PLATFORM).mk    prebuild 
	$(MAKE) -f Build/Rules/$(PLATFORM).mk -j build    
	$(MAKE) -f Build/Rules/$(PLATFORM).mk    postbuild
	$(MAKE) -f Build/Rules/Makefile generate_cc_json

clear:
	echo "Cleaning Intermediate/Build/$(PLATFORM)/Development/$(TARGET)"
	rm -rf Intermediate/Build/$(PLATFORM)/Development/$(TARGET)
	echo "Cleaning Intermediate/Build/$(PLATFORM)/Shipping/$(TARGET)"
	rm -rf Intermediate/Build/$(PLATFORM)/Shipping/$(TARGET)

clean:
	echo "Cleaning Everything"
	rm -rf .cache
	rm -rf Binaries
	rm -rf Intermediate
