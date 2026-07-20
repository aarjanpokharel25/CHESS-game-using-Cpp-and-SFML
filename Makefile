CXX       := g++
CXX_FLAGS := -std=c++17 -O2 -mwindows -static-libgcc -static-libstdc++

BIN     := bin
SRC     := src

# Path to your SFML SDK (change this if you installed SFML somewhere else)
SFML_DIR := C:/SFML

INCLUDE   := $(SFML_DIR)/include
LIB_PATH  := $(SFML_DIR)/lib
LIBRARIES := -lsfml-graphics -lsfml-window -lsfml-system
EXECUTABLE  := main


all: $(BIN) $(BIN)/$(EXECUTABLE) copy-dlls copy-textures

# Create the bin folder if it doesn't exist yet (this was the main problem!)
$(BIN):
	mkdir $(BIN)

# Copy the SFML DLLs next to the exe so Windows can find them at runtime
copy-dlls:
	cp -u $(SFML_DIR)/bin/openal32.dll $(BIN)/
	cp -u $(SFML_DIR)/bin/sfml-graphics-2.dll $(BIN)/
	cp -u $(SFML_DIR)/bin/sfml-window-2.dll $(BIN)/
	cp -u $(SFML_DIR)/bin/sfml-system-2.dll $(BIN)/

# Copy the game's images/font next to the exe (code loads them via relative "Textures/..." path)
copy-textures:
	cp -ur $(SRC)/Textures $(BIN)/

run: clean all
	cd $(BIN) && ./$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)/*.cpp
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) $^ -o $@ -L$(LIB_PATH) $(LIBRARIES)

clean:
	-rm -rf $(BIN)/*