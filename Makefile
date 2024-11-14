# Compiler
CC = gcc

# Compiler Flags
CFLAGS = -g -Wall -pthread

# Build folder
BUILD_DIR = build

# Output executable name (inside the build folder)
TARGET = chash

# Source files
SRCS = chash.c processinput.c rwlock.c timestamp.c

# Header files (for dependencies)
HDRS = chash.h processinput.h rwlock.h timestamp.h

# Object files (place them in the build folder)
OBJS = $(SRCS:%.c=$(BUILD_DIR)/%.o)

# Default target
all: $(TARGET)

# Create the build directory if it doesn't exist, then link all object files to create the executable
$(TARGET): $(OBJS) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)
	echo "\033[32mDone!\033[0m"

# Compile each .c file to .o file inside the build folder
$(BUILD_DIR)/%.o: %.c $(HDRS) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Create the build directory if it doesn't exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Clean up build artifacts
clean:
	rm -rf $(BUILD_DIR)

# Phony targets to avoid conflicts with files of the same name
.PHONY: all clean