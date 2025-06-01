SRC = engine/src/sound.cpp engine/src/file.cpp engine/src/helpers.cpp engine/src/logging.cpp engine/src/math.cpp engine/src/graphics.cpp
OBJ = $(SRC:.cpp=.obj)
INCLUDE = /Iengine/include
DLLNAME = arcane_engine.dll

CC = g++
CFLAGS = -std=c++17 -Wall -Wextra -O2 -Iengine/include -DBUILD_ARCANE_DLL -shared
LDFLAGS = -o $(DLLNAME) -luser32 -lgdi32 -ldsound -lpng

all: $(DLLNAME)

$(DLLNAME): $(SRC)
	$(CC) $(CFLAGS) $(SRC) $(LDFLAGS)

clean:
	rm $(DLLNAME)