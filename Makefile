# Compiler and flags
CXX := g++
CXXFLAGS := -Wall -Wextra -std=c++20 -I scripts

# Paths
SCRIPT_DIR := scripts
TEST_DIR := tests
BUILD_DIR := build

# Find all .cpp source files for scripts (shared code)
SCRIPT_SRCS := $(wildcard $(SCRIPT_DIR)/*.cpp)
SCRIPT_OBJS := $(patsubst $(SCRIPT_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SCRIPT_SRCS))

# Find all test source files
TEST_SRCS := $(wildcard $(TEST_DIR)/*.cpp)
TEST_BINS := $(patsubst $(TEST_DIR)/%.cpp,$(BUILD_DIR)/%,$(TEST_SRCS))

# Default target
.PHONY: all
all: $(TEST_BINS)

# Build rule for each test binary
$(BUILD_DIR)/%: $(TEST_DIR)/%.cpp $(SCRIPT_OBJS) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $< $(SCRIPT_OBJS) -o $@

# Build rule for scripts object files
$(BUILD_DIR)/%.o: $(SCRIPT_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Create build directory if missing
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Clean up build artifacts
.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)