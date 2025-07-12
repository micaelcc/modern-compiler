CC = gcc
CFLAGS = -Wall -Wextra -D_POSIX_C_SOURCE=200809L -I. $(shell pkg-config --cflags glib-2.0)

SRC_DIR = src
BIN_DIR = bin
BISON_DIR = $(SRC_DIR)/parser/implementations/bison

FLEX_C   = $(BISON_DIR)/lex.yy.c
BISON_C  = $(BISON_DIR)/parser.tab.c

LEX_OBJ    = $(BIN_DIR)/lex.yy.o
BISON_OBJ  = $(BIN_DIR)/parser.tab.o

SRCS = $(filter-out %_test.c, $(wildcard $(SRC_DIR)/**/*.c)) \
       $(filter-out %_test.c, $(wildcard $(SRC_DIR)/**/**/*.c)) \
       $(wildcard $(SRC_DIR)/*.c)

OBJS = $(patsubst $(SRC_DIR)/%.c,$(BIN_DIR)/%.o,$(filter-out $(FLEX_C) $(BISON_C),$(SRCS))) \
       $(LEX_OBJ) $(BISON_OBJ)

TARGET = main
LIBS = $(shell pkg-config --libs glib-2.0) -lm -lcjson -lfl

all: $(BIN_DIR)/$(TARGET)

$(BIN_DIR)/$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LIBS)

$(BIN_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(LEX_OBJ): $(FLEX_C)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BISON_OBJ): $(BISON_C)
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
		$(CC) $(CFLAGS) $(filter-out bin/main.o, $(OBJS)) $$testfile -o $(BIN_DIR)/$$testname $(LIBS); \
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
