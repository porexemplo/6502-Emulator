CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -g
SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build
TEST_DIR = tests
GTEST_LIBS = -lgtest -lgtest_main -pthread  # GoogleTest linking flags

SRCS = $(wildcard $(SRC_DIR)/*.cpp)
SRCS_NO_MAIN = $(filter-out $(SRC_DIR)/main.cpp, $(SRCS))
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))
OBJS_NO_MAIN = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS_NO_MAIN))
TARGET = bin/emulator

TEST_SRCS = $(wildcard $(TEST_DIR)/*.cpp)
TEST_OBJS = $(patsubst $(TEST_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(TEST_SRCS))
TEST_TARGET = bin/test

# Default build (builds emulator)
all: $(TARGET)

# Normal build
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

# Test build (EXCLUDES main.cpp)
$(BUILD_DIR)/%.o: $(TEST_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

$(TEST_TARGET): $(TEST_OBJS) $(OBJS_NO_MAIN)  # Use OBJS_NO_MAIN to exclude main.cpp
	$(CXX) $(CXXFLAGS) $(TEST_OBJS) $(OBJS_NO_MAIN) -o $(TEST_TARGET) $(GTEST_LIBS)

# Run tests
test: $(TEST_TARGET)
	./$(TEST_TARGET)

# Clean build files
clean:
	rm -rf $(BUILD_DIR) $(TARGET) $(TEST_TARGET)
