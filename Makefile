# Compiler
CC = gcc

# Compiler Flags
CFLAGS = -Wall -pthread

# Output executable name
TARGET = main

# Source files
SRCS = main.c chash.c processinput.c rwlock.c timestamp.c

# Header files (for dependencies)
HDRS = main.h chash.h processinput.h rwlock.h timestamp.h

# Object files
OBJS = $(SRCS:.c=.o)

# Default target
all: $(TARGET)

# Link all object files to create the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Compile each .c file to .o file
%.o: %.c $(HDRS)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build artifacts
clean:
	rm -f $(OBJS) $(TARGET)

# Phony targets to avoid conflicts with files of the same name
.PHONY: all clean
