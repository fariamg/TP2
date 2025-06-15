# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g
LDFLAGS =

# Directories
SRC_DIR = src
OBJ_DIR = build
BIN_DIR = bin
INCLUDE_DIR = include

# Find all .cpp files
SOURCES = $(wildcard $(SRC_DIR)/app/*.cpp) \
          $(wildcard $(SRC_DIR)/core/*.cpp) \
          $(wildcard $(SRC_DIR)/dataStructures/*.cpp) \
          $(wildcard $(SRC_DIR)/domains/*.cpp) \
          $(wildcard $(SRC_DIR)/io/*.cpp) \
          $(wildcard $(SRC_DIR)/ui/*.cpp) 

# Object files (replace src/ with obj/ and .cpp with .o)
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SOURCES))

# Executable name
EXECUTABLE = $(BIN_DIR)/simulation

# Default target
all: $(EXECUTABLE)

# Rule to link the executable
$(EXECUTABLE): $(OBJECTS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS)
	@echo "Linking complete: $@"

# Rule to compile source files into object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -c $< -o $@
	@echo "Compiled: $< -> $@"

# Clean rule
clean:
	@echo "Cleaning up..."
	rm -rf $(OBJ_DIR) $(BIN_DIR)
	@echo "Cleanup complete."

# Phony targets
.PHONY: all clean