# Makefile
CXX = g++           # Compiler
CXXFLAGS = -std=c++17 -g
TARGET = incrementalshortestpath.exe  # Executable name
SRC_DIR = allPairsSP       # Source file
INC_DIR = graphs 			#Include file
TEST_DIR = tests
# Tests

# Makefile rules
all: $(TARGET)


$(TARGET): incrementalshortestpath.o graphs.o 
	$(CXX) $(CXXFLAGS) -o $(TARGET) incrementalshortestpath.o graphs.o

incrementalshortestpath.o: allPairsSP\incrementalshortestpath.cpp  graphs\graphs.cpp
	$(CXX) $(CXXFLAGS) -c -o incrementalshortestpath.o allPairsSP\incrementalshortestpath.cpp 

graphs.o: graphs\graphs.cpp
	$(CXX) $(CXXFLAGS) -c -o graphs.o graphs\graphs.cpp


clean:
	del *.o $(TARGET)

.PHONY: all clean
