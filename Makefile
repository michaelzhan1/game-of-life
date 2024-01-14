CXX := g++
CXXFLAGS := -std=c++20 -Wall
GTKMMFLAGS := `pkg-config gtkmm-3.0 --cflags --libs`

SRC_DIR := src
BUILD_DIR := build
BIN_DIR := bin

SRCS := $(wildcard $(SRC_DIR)/*.cc)
OBJS := $(addprefix $(BUILD_DIR)/, $(notdir $(SRCS:.cc=.o)))

TARGET := $(BIN_DIR)/main

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cc
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $< $(GTKMMFLAGS)

clean:
	rm -rf $(BUILD_DIR) $(TARGET) $(BIN_DIR)

.PHONY: all clean