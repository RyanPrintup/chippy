CC = g++
CFLAGS = -std=c++11

BIN_DIR = bin
SRC_DIR = src

LIBS = -lSDL2
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(BIN_DIR)/%.o, $(SOURCES))

EXECUTABLE = Chippy

all: $(EXECUTABLE)

$(EXECUTABLE) : $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) $(LIBS) -o $(BIN_DIR)/$@

$(BIN_DIR)/%.o : $(SRC_DIR)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY clean

clean:
	rm -f $(BIN_DIR)/*.o $(BIN_DIR)/$(EXECUTABLE)
