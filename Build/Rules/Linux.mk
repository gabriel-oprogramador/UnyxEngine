include Build/Rules/Common.mk
include $(TARGET_MK)

BUILD_CONFIG := $(TARGET_KIND)|$(BUILD_TYPE)

DEFINES  += -DPLATFORM_LINUX -DTARGET_NAME=\"$(TARGET_NAME)\"
INCLUDES += -I$(SOURCE_DIR)
FLAGS    += -target x86_64-linux-gnu.$(LINUX_GLIBC_VERSION) 
FLAGS    += $(PROJECT_FLAGS) $(INCLUDES) $(DEFINES) 

ENABLE_DEBUG_FLAGS  := -DDEV_MODE -DDEBUG -O0 -g
DISABLE_DEBUG_FLAGS := -DSHIP_MODE -DNDEBUG -O2 -g0 -s

CC := $(ZIG_DIR)/zig cc
CX := $(ZIG_DIR)/zig c++
LD := $(ZIG_DIR)/zig c++

EXEC_OUTPUT_NAME  = $(TARGET_NAME)
STATIC_LIB_NAME   = lib$(TARGET_NAME).a
SHARED_LIB_NAME   = lib$(TARGET_NAME)-Module.so

EXEC_OUTPUT       = $(BIN_DIR)/$(EXEC_OUTPUT_NAME)
STATIC_LIB_OUTPUT = $(BIN_DIR)/$(STATIC_LIB_NAME)
SHARED_LIB_OUTPUT = $(BIN_DIR)/$(SHARED_LIB_NAME)

define LINK_STATIC_LIB
echo "  Linking Static Lib -> $(STATIC_LIB_OUTPUT)"
$(AR) rcs $(STATIC_LIB_OUTPUT) $(OBJ)
endef

define LINK_EXECUTABLE
echo "  Linking Executable -> $(EXEC_OUTPUT)"
$(LD) $(OBJ) $(FLAGS) $(LIBS) -o$(EXEC_OUTPUT) -Wl,-rpath,'$$ORIGIN'
endef

define LINK_SHARED_LIB
echo "  Linking Shared Lib -> $(SHARED_LIB_OUTPUT)"
$(LD) -shared $(OBJ) $(FLAGS) $(LIBS) -o$(SHARED_LIB_OUTPUT) -Wl,-soname,$(SHARED_LIB_NAME) -Wl,-rpath,'$$ORIGIN'
endef

ifeq ($(BUILD_CONFIG),ENGINE|Development)
SHARED_LIB_NAME := lib$(TARGET_NAME).so
FLAGS    += -DUNYX_ENGINE_BUILD
FLAGS    += $(ENABLE_DEBUG_FLAGS)
LIBS     +=
LINK     := $(LINK_SHARED_LIB)

else ifeq ($(BUILD_CONFIG),ENGINE|Shipping)
SHARED_LIB_NAME := lib$(TARGET_NAME).so
FLAGS    += -DUNYX_ENGINE_BUILD
FLAGS    += $(DISABLE_DEBUG_FLAGS)
LIBS     +=
LINK     := $(LINK_SHARED_LIB)

else ifeq ($(BUILD_CONFIG),GAME|Development)
LIBS     += -L$(BIN_DIR) -lUnyxEngine
FLAGS    += -isystem Source/Engine/Public
FLAGS    += $(ENABLE_DEBUG_FLAGS)
RUN_EXEC := ./$(EXEC_OUTPUT)
define LINK
$(LINK_SHARED_LIB)
$(LINK_EXECUTABLE) ./Build/Resources/GameMain.cpp
rm -f $(BIN_DIR)/GameMain.lib
endef

else ifeq ($(BUILD_CONFIG),GAME|Shipping)
LIBS     += -L$(BIN_DIR) -lUnyxEngine
FLAGS    += -isystem Source/Engine/Public
FLAGS    += $(DISABLE_DEBUG_FLAGS)
RUN_EXEC := ./$(EXEC_OUTPUT)
define LINK
$(LINK_EXECUTABLE) ./Build/Resources/GameMain.cpp
endef

else ifeq ($(BUILD_CONFIG),PROGRAM|Development)
LIBS     += -L$(BIN_DIR) -lUnyxEngine
FLAGS    += -isystem Source/Engine/Public
FLAGS    += $(ENABLE_DEBUG_FLAGS)
RUN_EXEC := ./$(EXEC_OUTPUT)
LINK     := $(LINK_EXECUTABLE)

else ifeq ($(BUILD_CONFIG),PROGRAM|Shipping)
LIBS     += -L$(BIN_DIR) -lUnyxEngine
FLAGS    += -isystem Source/Engine/Public
FLAGS    += $(DISABLE_DEBUG_FLAGS)
RUN_EXEC := ./$(EXEC_OUTPUT)
LINK     := $(LINK_EXECUTABLE)
endif

prebuild:
	echo "Start Build Target:$(TARGET_NAME), Platform:$(PLATFORM), Type:$(BUILD_TYPE)"
	mkdir -p $(BIN_DIR)
	mkdir -p $(DIRS)

build: $(OBJ)
	$(LINK)

postbuild:
	cp -f Build/Resources/AppIcon.png $(BIN_DIR)/AppIcon.png

run:
	echo "Running -> $(EXEC_OUTPUT)"
	$(RUN_EXEC)

$(OBJ_DIR)/%.o: $(SOURCE_DIR)/%.c
	echo "  Compiling > $<..."
	$(CC) $(FLAGS) -std=gnu11 -MMD -MP -MF $(@:.o=.d) -MT $@ -MJ$@.json -c $< -o $@

$(OBJ_DIR)/%.o: $(SOURCE_DIR)/%.cpp
	echo "  Compiling > $<..."
	$(CX) $(FLAGS) -std=gnu++17 -MMD -MP -MF $(@:.o=.d) -MT $@ -MJ$@.json -c $< -o $@

-include $(DEPS)
