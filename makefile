# Compiler and compiler flags
CC = gcc
CFLAGS = -Wall -pthread

# Define the target executable
TARGET = poker_simulator

# List of source files
SRCS = main.c hash.c poker.c
OBJS = $(SRCS:.c=.o)

# Default target
all: $(TARGET)

# Link object files into the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Compile source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $<

# Clean up generated files
clean:
	rm -f $(TARGET) $(OBJS)

# Additional dependencies
hash.o: hash.h
poker.o: poker.h
