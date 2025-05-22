CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Iinclude -Isrc
LDFLAGS := -Llib
LDLIBS := -lsfml-graphics -lsfml-window -lsfml-system -ltinyxml2

SRC_DIR := src
BUILD_DIR := build
BIN_DIR := .

SOURCES := $(wildcard $(SRC_DIR)/*.cpp) main.cpp
OBJECTS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(filter $(SRC_DIR)/%.cpp,$(SOURCES))) $(BUILD_DIR)/main.o

TARGET := $(BIN_DIR)/tiled_game

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $@ $(LDLIBS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/main.o: main.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(TARGET)