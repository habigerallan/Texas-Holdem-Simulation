CC = gcc
CFLAGS = -Iinclude -Wall -pthread
SRC_DIR = src
BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj

# Object files for main and worker
MAIN_OBJ = $(OBJ_DIR)/main.o $(OBJ_DIR)/card.o $(OBJ_DIR)/map.o $(OBJ_DIR)/texas_hold_em.o
WORKER_OBJ = $(OBJ_DIR)/worker.o $(OBJ_DIR)/card.o $(OBJ_DIR)/map.o $(OBJ_DIR)/texas_hold_em.o

# Define all targets that are not files
.PHONY: all clean

all: $(BUILD_DIR)/main $(BUILD_DIR)/worker

# Link the main program
$(BUILD_DIR)/main: $(MAIN_OBJ)
	$(CC) $(CFLAGS) $^ -o $@

# Link the worker program
$(BUILD_DIR)/worker: $(WORKER_OBJ)
	$(CC) $(CFLAGS) $^ -o $@

# Compile the source files to object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)
