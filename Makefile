CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -DNDEBUG

SRC_DIR = src
INC_DIR = include
TEST_DIR = tests
EXMPL_DIR = example
BUILD_DIR = build

SOURCES = $(SRC_DIR)/ring_buffer.c
OBJECTS = $(BUILD_DIR)/ring_buffer.o

TEST_SOURCE = $(TEST_DIR)/tests.c
EXMPL_SOURCE = $(EXMPL_DIR)/example.c

all: $(BUILD_DIR) $(BUILD_DIR)/test_rb $(BUILD_DIR)/example_rb

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# First dependency $<, target $@
$(BUILD_DIR)/ring_buffer.o: $(SRC_DIR)/ring_buffer.c $(INC_DIR)/ring_buffer.h
	$(CC) $(CFLAGS) -c $< -o $@

# Test executable
# $^ mean all dependencies
$(BUILD_DIR)/test_rb: $(TEST_SOURCE) $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@

# Example executable
$(BUILD_DIR)/example_rb: $(EXMPL_SOURCE) $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@

# Test run
test: $(BUILD_DIR)/test_rb
	./$(BUILD_DIR)/test_rb

# Example run
example: $(BUILD_DIR)/example_rb
	./$(BUILD_DIR)/example_rb

clean:
	rm -rf $(BUILD_DIR) .gdb_history

.PHONY: all clean test example