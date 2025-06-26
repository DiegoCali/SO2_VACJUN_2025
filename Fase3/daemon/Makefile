CC = gcc
CFLAGS = -Wall -pthread
TARGET = dist/main

# Object files will now live in build/
OBJ_DIR = build
SRC = main.c daemon.c web.c -ljson-c
OBJ = $(SRC:%.c=$(OBJ_DIR)/%.o)

all: $(TARGET)

# Rule to link the final binary
$(TARGET): $(OBJ)
	mkdir -p dist
	$(CC) $(CFLAGS) -o $@ $^

# Pattern rule to compile .c -> build/.o
$(OBJ_DIR)/%.o: %.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) dist
