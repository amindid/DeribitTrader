# Load environment variables from .env file
ifneq ("$(wildcard .env)","")
   include .env
   export
endif

# Compiler and linker flags
CXX = c++
CXXFLAGS = -g -std=c++17 -I./nlohmann/single_include -I./websocketpp -pthread -DBOOST_ASIO_NO_DEPRECATED -DASIO_STANDALONE
LDFLAGS = -pthread -lcurl -lboost_system -lssl -lcrypto  # Link with OpenSSL libraries here

# Source files
SRC = main.cpp utils.cpp request_senders.cpp actions.cpp streamer.cpp
OBJ = $(SRC:.cpp=.o)
EXEC = deribit_trader

# Default target to build the program
all: $(EXEC)

# Link the object files to create the executable
$(EXEC): $(OBJ)
	$(CXX) $(OBJ) -o $(EXEC) $(LDFLAGS)

# Compile the source code
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up object files and the executable
clean:
	rm -f $(OBJ) $(EXEC)

# Run the program
run: $(EXEC)
	./$(EXEC)
