CC = g++

BIN_DIR = bin
SRC_DIR = src

SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(BIN_DIR)/%.o, $(SOURCES))

EXECUTABLE = Chippy

all: $(EXECUTABLE)

$(EXECUTABLE) : $(OBJECTS)
	$(CC) $(OBJECTS) -o $(BIN_DIR)/$@

$(BIN_DIR)/%.o : $(SRC_DIR)/%.cpp
	$(CC) -c $< -o $@

clean:
	rm -f $(BIN_DIR)/*.o $(BIN_DIR)/$(EXECUTABLE)
