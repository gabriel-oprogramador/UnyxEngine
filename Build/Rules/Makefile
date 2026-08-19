.SILENT:
.PHONY: all dev start prebuild build postbuild clean
MAKEFLAGS += --no-print-directory
include Build/Rules/Common.mk

SELF_MK      = Build/Rules/Makefile
PLATFORM_MK  = Build/Rules/$(PLATFORM).mk

TARGET ?= Sandbox

all: dev

generate_cc_json:
	$(GENERATE_COMPILE_COMMANDS)

dev: 
	$(MAKE) -f $(SELF_MK) start TARGET=Engine    PLATFORM=$(PLATFORM) BUILD_TYPE=Development
	$(MAKE) -f $(SELF_MK) start TARGET=$(TARGET) PLATFORM=$(PLATFORM) BUILD_TYPE=Development

ship: 
	$(MAKE) -f $(SELF_MK) start TARGET=Engine    PLATFORM=$(PLATFORM) BUILD_TYPE=Shipping
	$(MAKE) -f $(SELF_MK) start TARGET=$(TARGET) PLATFORM=$(PLATFORM) BUILD_TYPE=Shipping

start:
	echo "Start Build Target:$(TARGET), Platform:$(PLATFORM), Type:$(BUILD_TYPE)"
	$(MAKE) -f Build/Rules/$(PLATFORM).mk    prebuild  TARGET=$(TARGET) BUILD_TYPE=$(BUILD_TYPE)
	$(MAKE) -f Build/Rules/$(PLATFORM).mk -j build     TARGET=$(TARGET) BUILD_TYPE=$(BUILD_TYPE)
	$(MAKE) -f Build/Rules/$(PLATFORM).mk    postbuild TARGET=$(TARGET) BUILD_TYPE=$(BUILD_TYPE)
	$(MAKE) -f Build/Rules/Makefile generate_cc_json   TARGET=$(TARGET) BUILD_TYPE=$(BUILD_TYPE)

run: dev
	$(MAKE) -f Build/Rules/$(PLATFORM).mk run TARGET=$(TARGET) BUILD_TYPE=Development

clean:
	echo "Cleaning Everything"
	rm -f compile_commands.json
	rm -rf .cache
	rm -rf Binaries
	rm -rf Intermediate
