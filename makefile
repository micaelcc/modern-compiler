CC = gcc
CFLAGS = -Wall -Wextra -I.

SRC_DIR = src
BIN_DIR = bin

SRCS = $(wildcard $(SRC_DIR)/**/*.c) $(wildcard $(SRC_DIR)/**/**/*.c)  $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c,$(BIN_DIR)/%.o,$(SRCS))

TARGET = main
all: $(BIN_DIR)/$(TARGET)

$(BIN_DIR)/$(TARGET): $(OBJS)
	$(CC) -o $@ $^

$(BIN_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BIN_DIR)/*.o $(BIN_DIR)/$(TARGET)
	rm -rf $(BIN_DIR)/constants $(BIN_DIR)/lex $(BIN_DIR)/repl $(BIN_DIR)/token $(BIN_DIR)/utils
