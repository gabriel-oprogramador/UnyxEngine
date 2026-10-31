# Unyx Target

TARGET_NAME = UnyxEngine
TARGET_KIND = ENGINE
SOURCE_DIR  = Source/Engine
INCLUDES   += -I $(SOURCE_DIR)/Public
INCLUDES   += -I $(SOURCE_DIR)/ThirdParty
DEFINES    += -D RENDERER_OPENGL
FLAGS      +=
LIBS       +=
ifeq ($(PLATFORM), Linux)
PKGS = x11 xi gl
PKG-LIB_LIB  = $(shell pkg-config --libs $(1))
PKG_INC_DIR  = $(sort $(shell pkg-config --variable=includedir $(1)))
PKG_LIB_DIR  = $(sort $(shell pkg-config --variable=libdir $(1)))
INCLUDES    += $(foreach d,$(call PKG_INC_DIR,$(PKGS)),-isystem $(d))
LIBS        += $(foreach d,$(call PKG_LIB_DIR,$(PKGS)),-L $(d))
LIBS        += $(call PKG-LIB_LIB,$(PKGS))
endif
