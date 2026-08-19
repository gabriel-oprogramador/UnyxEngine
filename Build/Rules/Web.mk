include Build/Rules/Common.mk
include $(TARGET_DIR)/$(TARGET)/Target.mk

DEFINES  += -DPLATFORM_WEB -DTARGET_NAME=\"$(TARGET_NAME)\"
INCLUDES += -I$(SOURCE_DIR)
FLAGS    += $(PROJECT_FLAGS) $(INCLUDES) $(DEFINES) 

CC := $(EMSCRIPTEN_DIR)/emcc
CX := $(EMSCRIPTEN_DIR)/em++
LD := $(EMSCRIPTEN_DIR)/em++
AR := $(EMSCRIPTEN_DIR)/emar
ER := $(EMSCRIPTEN_DIR)/emrun

ifeq ($(TARGET_KIND), ENGINE)

define LINK
echo "Linking -> $(LIB_OUTPUT)"
$(LINK_STATIC_LIB)
endef

else ifeq ($(TARGET_KIND), GAME)
LIBS  += -sUSE_WEBGL2=1 -sFULL_ES3 -sWASM=1 -sASYNCIFY
LIBS  += --preload-file Content@/Content/ -sALLOW_MEMORY_GROWTH=1 -sINITIAL_MEMORY=512mb
LIBS  += -L$(BIN_DIR) -lUnyxEngine
FLAGS += -isystem Source/Engine/Public

define LINK
echo "Linking -> $(BIN_OUTPUT)"
$(LINK_EXECUTABLE)
endef

define RUN
echo "Running -> $(BIN_OUTPUT)"
$(NODE) $(LIVE_SERVER) $(BIN_DIR)
endef

endif

OUTPUT_NAME     = $(TARGET_NAME)
LIB_NAME        = lib$(TARGET_NAME).a
BIN_OUTPUT      = $(BIN_DIR)/index.js
LIB_OUTPUT      = $(BIN_DIR)/$(LIB_NAME)
LINK_STATIC_LIB = $(AR) rcs $(LIB_OUTPUT) $(OBJ)
LINK_EXECUTABLE = $(LD) $(OBJ) $(FLAGS) $(LIBS) -o$(BIN_OUTPUT)

prebuild:
	mkdir -p $(BIN_DIR)
	mkdir -p $(DIRS)

build: $(OBJ)
	$(LINK)

postbuild:
	cp ./Build/Resources/WebEntrypoint.html $(BIN_DIR)/index.html
	cp ./Build/Resources/AppIcon.png $(BIN_DIR)/AppIcon.ico

run:
	$(RUN)

$(OBJ_DIR)/%.o: $(SOURCE_DIR)/%.c
	echo "Compiling > $<..."
	$(CC) $(FLAGS) -std=gnu11 -MMD -MJ$@.json -c $< -o $@

$(OBJ_DIR)/%.o: $(SOURCE_DIR)/%.cpp
	echo "Compiling > $<..."
	$(CX) $(FLAGS) -std=gnu++17 -MD -MP -MJ$@.json -c $< -o $@

-include $(DEPS)
