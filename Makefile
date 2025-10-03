CXX = g++
CXXFLAGS = -std=c++17 -g -Wall -Wextra

# Target executable
TARGET = solution

# Source files
SRC = solution.cpp

# Object files
OBJ = $(SRC:.cpp=.o)

# Default target
all: $(TARGET)

# Build the executable
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ)

# Compile source files to object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Run tests using pytest
test: $(TARGET)
	pytest test_game.py -v

# Clean compiled files
clean:
	rm -f $(TARGET) $(OBJ)

# Phony targets (not actual files)
.PHONY: all test clean
