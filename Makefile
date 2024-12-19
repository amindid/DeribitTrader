# Load environment variables from .env file
ifneq ("$(wildcard .env)","")
   include .env
   export
endif

# Compiler and linker flags
CXX = g++
CXXFLAGS = -g -std=c++11
LDFLAGS = -lcurl  # Link with libcurl

# Source files
SRC = main.cpp
OBJ = $(SRC:.cpp=.o)
EXEC = deribit_trader

# Default target to build the program
all: $(EXEC)

# Link the object files to create the executable
$(EXEC): $(OBJ)
	$(CXX) $(OBJ) -o $(EXEC) $(LDFLAGS)

# Compile the source code
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

# Clean up object files and the executable
clean:
	rm -f $(OBJ) $(EXEC)

# Run the program
run: $(EXEC)
	./$(EXEC)

