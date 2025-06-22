# Compiler and flags
CXX = g++
CXXFLAGS = -g -o -Wall -Wextra -std=c++17 -Iinclude
LDFLAGS =

# Directories
SRCDIR = src
OBJDIR = obj
BINDIR = bin

# Executables
MAIN_EXECUTABLE = $(BINDIR)/tp2.out
ANALYSIS_EXECUTABLE = $(BINDIR)/analysis.out

# Find all .cpp files recursively in SRCDIR, excluding analysis/ExperimentRunner.cpp
MAIN_SOURCES = $(shell find $(SRCDIR) -name '*.cpp' ! -path '$(SRCDIR)/analysis/ExperimentRunner.cpp')
ANALYSIS_SOURCES = $(shell find $(SRCDIR) -name '*.cpp' ! -path '$(SRCDIR)/app/main.cpp')

# Generate .o file names from .cpp file names
MAIN_OBJECTS = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(MAIN_SOURCES))
ANALYSIS_OBJECTS = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(ANALYSIS_SOURCES))

# Default target: 'all'
all: $(MAIN_EXECUTABLE) $(ANALYSIS_EXECUTABLE)

# Rule to link the main executable
$(MAIN_EXECUTABLE): $(MAIN_OBJECTS)
	@mkdir -p $(BINDIR) 
	$(CXX) $(LDFLAGS) $^ -o $@
	@echo "Executable $(MAIN_EXECUTABLE) created successfully."

# Rule to link the analysis executable
$(ANALYSIS_EXECUTABLE): $(ANALYSIS_OBJECTS)
	@mkdir -p $(BINDIR)
	$(CXX) $(LDFLAGS) $^ -o $@
	@echo "Analysis executable $(ANALYSIS_EXECUTABLE) created successfully."

# Rule to compile .cpp files to .o files
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(dir $@) 
	$(CXX) $(CXXFLAGS) -c $< -o $@
	@echo "Compiled $< to $@"

# Target to run analysis experiments
analysis: $(ANALYSIS_EXECUTABLE)
	@echo "Running experimental analysis..."
	./$(ANALYSIS_EXECUTABLE)
	@echo "Analysis complete. Check results/ directory for outputs."

# Clean target: removes obj and bin directories
clean:
	@echo "Cleaning up object and binary files..."
	rm -rf $(OBJDIR) $(BINDIR) results/
	@echo "Cleanup complete."

# Phony targets: targets that are not actual files
.PHONY: all clean analysis