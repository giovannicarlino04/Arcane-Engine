# Variables
CXX = g++
CXXFLAGS = -I./engine/include/ -mwindows
LDFLAGS = -static -luser32 -lgdi32 -ldsound -lxinput -lpng -ljpeg -lz -lstdc++
SRC = ./arcane_engine_template.cpp \
      ./engine/src/graphics.cpp \
      ./engine/src/sound.cpp \
      ./engine/src/helpers.cpp \
      ./engine/src/file.cpp \
      ./engine/src/math.cpp \
      ./engine/src/logging.cpp
OBJ_DIR = ./obj
BUILD_DIR = ./build
OBJ = $(patsubst ./%.cpp,$(OBJ_DIR)/%.o,$(SRC))
OUT = arcane_engine_template.exe

# Default target
all: $(BUILD_DIR)/$(OUT)

# Build the executable
$(BUILD_DIR)/$(OUT): $(OBJ)
	$(shell mkdir -p $(BUILD_DIR))
	$(CXX) $(CXXFLAGS) $(OBJ) -o $@ $(LDFLAGS)

# Pattern rule for object files
$(OBJ_DIR)/%.o: ./%.cpp
	$(shell mkdir -p $(dir $@))
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up
clean:
	rm -rf $(BUILD_DIR) $(OBJ_DIR)

.PHONY: all clean
