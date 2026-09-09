include Build/Rules/Common.mk
include $(TARGET_MK)

BUILD_CONFIG := $(TARGET_KIND)|$(BUILD_TYPE)

DEFINES  += -DPLATFORM_WEB -DTARGET_NAME=\"$(TARGET_NAME)\"
INCLUDES += -I$(SOURCE_DIR)
FLAGS    += $(PROJECT_FLAGS) $(INCLUDES) $(DEFINES)

ENABLE_DEBUG_FLAGS  := -DDEV_MODE -DDEBUG -O0 -g
DISABLE_DEBUG_FLAGS := -DSHIP_MODE -DNDEBUG -O2 -g0

CC := $(EMSCRIPTEN_DIR)/emcc
CX := $(EMSCRIPTEN_DIR)/em++
LD := $(EMSCRIPTEN_DIR)/em++
AR := $(EMSCRIPTEN_DIR)/emar

EXEC_OUTPUT_NAME  = index.js
STATIC_LIB_NAME   = lib$(TARGET_NAME).a
EXEC_OUTPUT       = $(BIN_DIR)/$(EXEC_OUTPUT_NAME)
STATIC_LIB_OUTPUT = $(BIN_DIR)/$(STATIC_LIB_NAME)

define LINK_STATIC_LIB
echo "  Linking Static Lib -> $(STATIC_LIB_OUTPUT)"
$(AR) rcs $(STATIC_LIB_OUTPUT) $(OBJ)
endef

define LINK_EXECUTABLE
echo "  Linking Executable -> $(EXEC_OUTPUT)"
$(LD) $(OBJ) $(FLAGS) $(LIBS) -o$(EXEC_OUTPUT)  ./Build/Resources/GameMain.cpp
cp ./Build/Resources/WebEntrypoint.html $(BIN_DIR)/index.html
cp ./Build/Resources/AppIcon.ico $(BIN_DIR)/AppIcon.ico
endef

ifeq ($(TARGET_KIND), PROGRAM)
$(error [Unyx] => $(TARGET_NAME) <= PROGRAM targets cannot be built for Web)
endif

ifeq ($(BUILD_CONFIG),ENGINE|Development)
FLAGS += -DUNYX_ENGINE_BUILD
FLAGS += $(ENABLE_DEBUG_FLAGS)
LINK  += $(LINK_STATIC_LIB)

else ifeq ($(BUILD_CONFIG),ENGINE|Shipping)
FLAGS    += -DUNYX_ENGINE_BUILD
FLAGS    += $(DISABLE_DEBUG_FLAGS)
LINK     += $(LINK_STATIC_LIB)

else ifeq ($(BUILD_CONFIG),GAME|Development)
LIBS     += -sUSE_WEBGL2=1 -sFULL_ES3 -sWASM=1 -sASYNCIFY
LIBS     += --preload-file Content@/Content/ -sALLOW_MEMORY_GROWTH=1 -sINITIAL_MEMORY=512mb
LIBS     += -L$(BIN_DIR) -lUnyxEngine
FLAGS    += -isystem Source/Engine/Public
FLAGS    += -DUNYX_APP_IMPLEMENTATION
FLAGS    += $(ENABLE_DEBUG_FLAGS)
RUN_EXEC := $(NODE) $(LIVE_SERVER) $(BIN_DIR)
LINK     := $(LINK_EXECUTABLE)

else ifeq ($(BUILD_CONFIG),GAME|Shipping)
LIBS     += -sUSE_WEBGL2=1 -sFULL_ES3 -sWASM=1 -sASYNCIFY
LIBS     += --preload-file Content@/Content/ -sALLOW_MEMORY_GROWTH=1 -sINITIAL_MEMORY=512mb
LIBS     += -L$(BIN_DIR) -lUnyxEngine
FLAGS    += -isystem Source/Engine/Public
FLAGS    += -DUNYX_APP_IMPLEMENTATION
FLAGS    += $(DISABLE_DEBUG_FLAGS)
RUN_EXEC := $(NODE) $(LIVE_SERVER) $(BIN_DIR)
LINK     := $(LINK_EXECUTABLE)

endif

prebuild:
	echo "Start Build Target:$(TARGET_NAME), Platform:$(PLATFORM), Type:$(BUILD_TYPE)"
	mkdir -p $(BIN_DIR)
	mkdir -p $(DIRS)

build: $(OBJ)
	$(LINK)

postbuild:

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
