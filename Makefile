# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++11 -Wall -g

# Libraries
LIBS = -lssl -lcrypto

# Source files for the main executable
SRCS = main.cpp Calculator.cpp ClientCommunicate.cpp Interface.cpp Error.cpp ConnectToBase.cpp

# Header files
HDRS = Calculator.h ClientCommunicate.h Interface.h Error.h ConnectToBase.h

# Executable name
EXECUTABLE = server

# Default target
all: $(EXECUTABLE)

# Rule to compile the main program
$(EXECUTABLE): $(SRCS) $(HDRS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(EXECUTABLE) $(LIBS)

# Rule to clean
clean:
	rm -f $(EXECUTABLE)

.PHONY: all clean