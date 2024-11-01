# Variables
CXX = g++
CXXFLAGS = -I./engine/include/
LDFLAGS = -static -luser32 -lgdi32 -ldsound -lxinput -lpng -ljpeg -ljpeg -lz -lstdc++
SRC = ./arcane_engine_template.cpp ./engine/src/graphics.cpp ./engine/src/sound.cpp ./engine/src/helpers.cpp ./engine/src/file.cpp ./engine/src/math.cpp
OBJ_DIR = ./obj
OBJ = $(patsubst ./engine/src/%.cpp,$(OBJ_DIR)/%.o,$(filter ./engine/src/%.cpp,$(SRC))) $(OBJ_DIR)/arcane_engine_template.o
OUT = arcane_engine_template.exe
BUILD_DIR = build

all: $(BUILD_DIR)/$(OUT)

# Build the executable
$(BUILD_DIR)/$(OUT): $(OBJ)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(BUILD_DIR)/$(OUT) $(LDFLAGS)

$(OBJ_DIR)/%.o: ./engine/src/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/arcane_engine_template.o: ./arcane_engine_template.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OUT) $(OBJ)

.PHONY: all clean
