# Variables
CXX = g++
CXXFLAGS = -I./engine/include/
LDFLAGS = -luser32 -lgdi32 -ldsound -lxinput -lpng
SRC = ./win32_engine_test.cpp ./engine/src/graphics.cpp ./engine/src/sound.cpp ./engine/src/helpers.cpp
OBJ_DIR = ./engine/obj
OBJ = $(patsubst ./engine/src/%.cpp,$(OBJ_DIR)/%.o,$(filter ./engine/src/%.cpp,$(SRC))) $(OBJ_DIR)/win32_engine_test.o
OUT = win32_engine_test.exe
BUILD_DIR = build

all: $(BUILD_DIR)/$(OUT)

# Build the executable
$(BUILD_DIR)/$(OUT): $(OBJ)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(BUILD_DIR)/$(OUT) $(LDFLAGS)

$(OBJ_DIR)/%.o: ./engine/src/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/win32_engine_test.o: ./win32_engine_test.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(OBJ_DIR)

.PHONY: all clean
