CC = gcc
CFLAGS = -Wall -Wextra -I.

SRC_DIR = src
BIN_DIR = bin

SRCS = $(filter-out %_test.c, $(wildcard $(SRC_DIR)/**/*.c)) $(filter-out %_test.c, $(wildcard $(SRC_DIR)/**/**/*.c))  $(wildcard $(SRC_DIR)/*.c)
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

TESTFILES := $(shell find ./src -name '*_test.c' -type f)

test:
	@echo "Building tests..."

	@for testfile in $(TESTFILES); do \
		testname=$$(basename $$testfile .c); \
		$(CC) $(CFLAGS) $(filter-out bin/main.o, $(OBJS)) $$testfile -o $(BIN_DIR)/$$testname; \
		echo "Run $$testname..."; \
		if ! $(BIN_DIR)/$$testname; then \
			failed="$$failed\n$$testname"; \
		fi; \
	done; \

	@if [ -n "$$failed" ]; then \
		echo "The following tests failed: $$failed"; \
		exit 1; \
	else \
		echo "All tests passed!"; \
	fi
