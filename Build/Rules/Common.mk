.SILENT:
.PHONY: all dev start prebuild build postbuild clean
MAKEFLAGS += --no-print-directory
LINUX_GLIBC_VERSION := 2.29

TOOLCHAIN_DIR = Toolchain
ZIG_DIR = $(TOOLCHAIN_DIR)/Zig/0.15.2
EMSDK_DIR = $(TOOLCHAIN_DIR)/Emsdk
EMSCRIPTEN_DIR = $(EMSDK_DIR)/upstream/emscripten
LIVE_SERVER = $(TOOLCHAIN_DIR)/Live-Server/node_modules/live-server/live-server.js

BUILD_TYPE ?= Development

# Setup Host
UNAME := $(shell uname -s | tr A-Z a-z)
ifeq ($(OS), Windows_NT)
PLATFORM ?= Windows
NODE = $(firstword $(wildcard $(EMSDK_DIR)/node/*/node.exe))
USER_DATA_DIR := $(subst \,/,$(LOCALAPPDATA))
else ifeq ($(UNAME), linux)
PLATFORM ?= Linux
NODE = $(firstword $(wildcard $(EMSDK_DIR)/node/*/bin/node))
USER_DATA_DIR := $(if $(XDG_DATA_HOME),$(XDG_DATA_HOME),$(HOME)/.local/share)
else 
$(error Host not supported)
endif

# Outputs
BIN_DIR      = Binaries/$(PLATFORM)/$(BUILD_TYPE)
OBJ_DIR      = Intermediate/Build/$(PLATFORM)/$(BUILD_TYPE)/$(TARGET_NAME)
PROJECT_PATH = $(subst \,/,$(CURDIR))

# Project Files
SRC_CC = $(shell find $(SOURCE_DIR) -type f -name "*.c")
SRC_CX = $(shell find $(SOURCE_DIR) -type f -name "*.cpp")
OBJ_CC = $(patsubst $(SOURCE_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_CC))
OBJ_CX = $(patsubst $(SOURCE_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_CX))
SRC    = $(SRC_CC) $(SRC_CX)
OBJ    = $(OBJ_CC) $(OBJ_CX)
DEPS   = $(OBJ:.o=.d)
DIRS   = $(dir $(OBJ))

# Project Infos
PROJECT_FLAGS += -DPROJECT_NAME=\"$(notdir $(CURDIR))\"
PROJECT_FLAGS += -DPROJECT_PATH=\"$(PROJECT_PATH)\"

JSONS = $(shell find Intermediate/Build/$(PLATFORM)/$(BUILD_TYPE) -type f -name "*.json")
define GENERATE_COMPILE_COMMANDS
echo "[" > compile_commands.json
for f in $(JSONS); do \
  cat $$f >> compile_commands.json; \
done
sed -i '$$ s/,$$//' compile_commands.json
echo "]" >> compile_commands.json
endef
