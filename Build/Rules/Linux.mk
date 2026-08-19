include Build/Rules/Common.mk
include $(TARGET_DIR)/$(TARGET)/Target.mk

DEFINES  += -DPLATFORM_LINUX -DTARGET_NAME=\"$(TARGET_NAME)\"
INCLUDES += -I$(SOURCE_DIR)
FLAGS    += -target x86_64-linux-gnu.$(LINUX_GLIBC_VERSION) 
FLAGS    += $(PROJECT_FLAGS) $(INCLUDES) $(DEFINES) 

CC := $(ZIG_DIR)/zig cc
CX := $(ZIG_DIR)/zig c++
LD := $(ZIG_DIR)/zig c++

ifeq ($(TARGET_KIND), ENGINE)

define LINK
echo "Linking -> $(LIB_OUTPUT)"
$(LINK_SHARED_LIB)
endef

else ifeq ($(TARGET_KIND), GAME)
LIBS  += -L$(BIN_DIR) -lUnyxEngine
FLAGS += -isystem Source/Engine/Public

define LINK
#echo "Linking -> $(BIN_OUTPUT)"
$(LINK_EXECUTABLE)
endef

define RUN
echo "Running -> $(BIN_OUTPUT)"
./$(BIN_OUTPUT)
endef
endif

OUTPUT_NAME     = $(TARGET_NAME)
LIB_NAME        = lib$(TARGET_NAME).so
BIN_OUTPUT      = $(BIN_DIR)/$(OUTPUT_NAME)
LIB_OUTPUT      = $(BIN_DIR)/$(LIB_NAME)
LINK_SHARED_LIB = $(LD) -shared $(OBJ) $(FLAGS) $(LIBS) -o$(LIB_OUTPUT) -Wl,-soname,$(LIB_NAME) -Wl,-rpath,'$$ORIGIN'
LINK_EXECUTABLE = $(LD) $(OBJ) $(FLAGS) $(LIBS) -o$(BIN_OUTPUT) -Wl,-rpath,'$$ORIGIN'

prebuild:
	mkdir -p $(BIN_DIR)
	mkdir -p $(DIRS)

build: $(OBJ)
	$(LINK)

postbuild:
	cp -f Build/Resources/AppIcon.png $(BIN_DIR)/AppIcon.png

run:
	$(RUN)

$(OBJ_DIR)/%.o: $(SOURCE_DIR)/%.c
	echo "Compiling > $<..."
	$(CC) $(FLAGS) -std=gnu11 -MMD -MJ$@.json -c $< -o $@

$(OBJ_DIR)/%.o: $(SOURCE_DIR)/%.cpp
	echo "Compiling > $<..."
	$(CX) $(FLAGS) -std=gnu++17 -MD -MP -MJ$@.json -c $< -o $@

-include $(DEPS)
