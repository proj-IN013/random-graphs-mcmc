CC = gcc
CFLAGS = -Iinclude -Wall -Wextra -Werror -Wno-unused-parameter

SRC_DIR = src
BUILD_DIR = build

PROGRAM = $(BUILD_DIR)/main

SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC))

all: $(PROGRAM)

$(PROGRAM): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)

run: $(PROGRAM)
	./$(PROGRAM)

total: clean all

.PHONY: all clean run re