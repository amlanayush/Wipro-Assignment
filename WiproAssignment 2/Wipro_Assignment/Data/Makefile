CXX = g++
CXXFLAGS = -std=c++17 -Wall -Isrc

SRC = $(wildcard src/*.cpp)
OBJ = $(SRC:.cpp=.o)

TARGET = shell

all: $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET)

clean:
	rm -f src/*.o $(TARGET)
