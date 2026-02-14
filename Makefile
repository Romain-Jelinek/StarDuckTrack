# Compiler and flags
CXX := g++
CXXFLAGS := -Wall -Wextra -std=c++20 -I src

# Paths
SRC_DIR := src
TEST_DIR := tests
BUILD_DIR := build

# Find all .cpp source files for SRCs (shared code)
SRC_SRCS := $(wildcard $(SRC_DIR)/*.cpp)
SRC_OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRC_SRCS))

# Find all test source files
TEST_SRCS := $(wildcard $(TEST_DIR)/*.cpp)
TEST_BINS := $(patsubst $(TEST_DIR)/%.cpp,$(BUILD_DIR)/%,$(TEST_SRCS))

# Default target
.PHONY: all
all: $(TEST_BINS)

# Build rule for each test binary
$(BUILD_DIR)/%: $(TEST_DIR)/%.cpp $(SRC_OBJS) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $< $(SRC_OBJS) -o $@

# Build rule for SRCs object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Create build directory if missing
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Clean up build artifacts
.PHONY: clean
clean:
	rm -r $(BUILD_DIR)