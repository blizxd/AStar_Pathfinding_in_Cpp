# Compiler and flags
CC := g++
CFLAGS := -g -Wall -Wextra
INCLUDE_DIRS := -ISDL2/include/SDL2
LDFLAGS := -L./SDL2/lib -lmingw32 -lSDL2main -lSDL2 

# Directories
SRC_DIR := .
OBJ_DIR := build/obj
BIN_DIR := build/bin

# Source and object files
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
HEADER_FILES := $(wildcard $(SRC_DIR)/*.h) $(wildcard $(SRC_DIR)/*.hpp)

# Executable name
EXECUTABLE := $(BIN_DIR)/main.exe

# Default target
all: $(EXECUTABLE)

run:
	cd $(BIN_DIR); main.exe

$(EXECUTABLE): $(SRC_FILES) $(HEADER_FILES)
	$(CC)  $(CFLAGS) $(INCLUDE_DIRS)  $(SRC_FILES) $(LDFLAGS) -o $(EXECUTABLE)

# Clean build artifacts
clean:
	rm -rf $(OBJ_DIR)/* $(BIN_DIR)/*.exe 

# Prevent make from treating these targets as files
.PHONY: all directories run clean distclean help test assets