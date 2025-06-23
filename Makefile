CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
INCLUDES = -Iinclude
SRCDIR = src
OBJDIR = obj
BINDIR = bin

# Source files for base implementation (excluding experiment runners and config generator main)
BASE_SOURCES = $(wildcard $(SRCDIR)/core/*.cpp) \
               $(wildcard $(SRCDIR)/dataStructures/*.cpp) \
               $(wildcard $(SRCDIR)/domains/*.cpp) \
               $(SRCDIR)/utils/Logger.cpp \
               $(SRCDIR)/utils/LoggerExtended.cpp \
               $(SRCDIR)/analysis/ConfigGenerator.cpp \
               $(SRCDIR)/analysis/SimulationMetrics.cpp

# Source files for extended implementation  
EXTENDED_SOURCES = $(SRCDIR)/core/ConfigDataExtended.cpp \
                   $(SRCDIR)/core/SchedulerExtended.cpp \
                   $(SRCDIR)/dataStructures/GraphExtended.cpp \
                   $(SRCDIR)/domains/PackageExtended.cpp \
                   $(SRCDIR)/domains/WarehouseExtended.cpp \
                   $(SRCDIR)/utils/LoggerExtended.cpp \
                   $(SRCDIR)/analysis/ConfigGeneratorExtended.cpp

# Object files
BASE_OBJECTS = $(BASE_SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
EXTENDED_OBJECTS = $(EXTENDED_SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)

# Targets
MAIN_BASE = $(BINDIR)/simulation_base
MAIN_EXTENDED = $(BINDIR)/simulation_extended
EXPERIMENT_BASE = $(BINDIR)/experiment_base
EXPERIMENT_EXTENDED = $(BINDIR)/experiment_extended
COMPARISON = $(BINDIR)/comparison
CONFIG_GENERATOR = $(BINDIR)/config_generator

.PHONY: all clean setup base extended experiments comparison configs

all: setup base extended experiments comparison

setup:
	@mkdir -p $(OBJDIR)/core $(OBJDIR)/dataStructures $(OBJDIR)/domains $(OBJDIR)/utils $(OBJDIR)/analysis $(OBJDIR)/app
	@mkdir -p $(BINDIR)
	@mkdir -p results configs

# Base implementation
base: $(MAIN_BASE)

$(MAIN_BASE): $(BASE_OBJECTS) $(OBJDIR)/app/main.o
	$(CXX) $(CXXFLAGS) -o $@ $^

# Extended implementation
extended: $(MAIN_EXTENDED)

$(MAIN_EXTENDED): $(BASE_OBJECTS) $(EXTENDED_OBJECTS) $(OBJDIR)/app/mainExtended.o
	$(CXX) $(CXXFLAGS) -o $@ $^

# Experiments
experiments: $(EXPERIMENT_BASE) $(EXPERIMENT_EXTENDED)

$(EXPERIMENT_BASE): $(BASE_OBJECTS) $(OBJDIR)/analysis/ExperimentRunner.o
	$(CXX) $(CXXFLAGS) -o $@ $^

$(EXPERIMENT_EXTENDED): $(BASE_OBJECTS) $(EXTENDED_OBJECTS) $(OBJDIR)/analysis/ExperimentRunnerExtended.o
	$(CXX) $(CXXFLAGS) -o $@ $^

# Comparison
comparison: $(COMPARISON)

$(COMPARISON): $(BASE_OBJECTS) $(EXTENDED_OBJECTS) $(OBJDIR)/analysis/ComparisonRunner.o
	$(CXX) $(CXXFLAGS) -o $@ $^

# Config generator
config-generator: $(CONFIG_GENERATOR)

$(CONFIG_GENERATOR): $(BASE_OBJECTS) $(OBJDIR)/utils/ConfigGeneratorMain.o
	$(CXX) $(CXXFLAGS) -o $@ $^

# Pattern rule for object files
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(OBJDIR) $(BINDIR)
	rm -rf results configs

# Configuration generation
configs: setup config-generator
	@echo "Gerando arquivos de configuração..."
	./$(CONFIG_GENERATOR)

test-base: base configs
	./$(MAIN_BASE) configs/test_config.txt

test-extended: extended configs  
	./$(MAIN_EXTENDED) configs/test_config_extended.txt

run-experiments: experiments
	./$(EXPERIMENT_BASE)
	./$(EXPERIMENT_EXTENDED)

run-comparison: comparison
	./$(COMPARISON)

help:
	@echo "Available targets:"
	@echo "  all         - Build everything (base, extended, experiments, comparison)"
	@echo "  base        - Build base simulation"
	@echo "  extended    - Build extended simulation with extra features"
	@echo "  experiments - Build experiment runners"
	@echo "  comparison  - Build comparison tool"
	@echo "  config-generator - Build configuration generator"
	@echo "  configs     - Generate configuration files"
	@echo "  test-base   - Run base simulation with test config"
	@echo "  test-extended - Run extended simulation with test config"
	@echo "  run-experiments - Run all experiments"
	@echo "  run-comparison - Run comparison analysis"
	@echo "  clean       - Remove all built files"