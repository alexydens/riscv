PROJECT_NAME=riscv
SRC_DIR=src
INC_DIR=include
OBJ_DIR=obj
BIN_DIR=bin

CFLAGS = -Wall -Wextra -Wpedantic -Werror -std=c11 -I$(INC_DIR)
CFLAGS += -Wno-unused
LDFLAGS = -lm

SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SOURCES))

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@
$(BIN_DIR)/$(PROJECT_NAME): $(OBJECTS) | $(BIN_DIR)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

$(OBJ_DIR):
	mkdir -p $@
$(BIN_DIR):
	mkdir -p $@

.PHONY: clean build test

build: $(BIN_DIR)/$(PROJECT_NAME)

clean:
	rm -rf $(OBJ_DIR)
	rm -rf $(BIN_DIR)

test: build
	./$(BIN_DIR)/$(PROJECT_NAME)
