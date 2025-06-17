# Compiler and flags
CXX = g++
CXXFLAGS = -g -o -Wall -Wextra -std=c++17 -Iinclude
LDFLAGS =

# Directories
SRCDIR = src
OBJDIR = obj
BINDIR = bin

# Executable name
EXECUTABLE = $(BINDIR)/tp2.out

# Find all .cpp files recursively in SRCDIR
# This will find files like src/app/main.cpp, src/utils/IO.cpp, etc.
SOURCES = $(shell find $(SRCDIR) -name '*.cpp')

# Generate .o file names from .cpp file names, placing them in OBJDIR,
# preserving the subdirectory structure from SRCDIR.
# e.g., src/app/main.cpp -> obj/app/main.o
OBJECTS = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SOURCES))

# Default target: 'all'
all: $(EXECUTABLE)

# Rule to link the executable
$(EXECUTABLE): $(OBJECTS)
	@mkdir -p $(BINDIR) 
	$(CXX) $(LDFLAGS) $^ -o $@
	@echo "Executable $(EXECUTABLE) created successfully."

# Rule to compile .cpp files to .o files
# This pattern rule handles all .cpp files found by the SOURCES variable.
# It creates the necessary subdirectory structure within OBJDIR.
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(dir $@) 
	$(CXX) $(CXXFLAGS) -c $< -o $@
	@echo "Compiled $< to $@"

# Clean target: removes obj and bin directories
clean:
	@echo "Cleaning up object and binary files..."
	rm -rf $(OBJDIR) $(BINDIR)
	@echo "Cleanup complete."

# Phony targets: targets that are not actual files
.PHONY: all clean