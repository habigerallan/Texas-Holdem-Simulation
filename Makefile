# Makefile

# Compiler and compiler flags
CC = gcc
CFLAGS = -Wall -Iinclude

# Define the source, object, and binary directories
SRCDIR = src
OBJDIR = build
BINDIR = build

# Define the target executable
TARGET = $(BINDIR)/TexasHoldEm

# Define source and object files
SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

# Default target
all: $(TARGET)

# Link the target executable
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $^ -g -o $@ -lpthread

# Compile the source files into object files
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -g -o $@

# Clean target
clean:
	rm -f $(OBJECTS) $(TARGET)

# Phony targets
.PHONY: all clean
